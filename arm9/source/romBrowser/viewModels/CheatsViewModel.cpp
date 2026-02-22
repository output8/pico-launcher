#include "common.h"
#include "cheats/ICheatRepository.h"
#include "fat/File.h"
#include "CheatsViewModel.h"

#define CRCPOLY 0xEDB88320

static u32 crc32(const void* buffer, u32 length)
{
    u32 crc = ~0u;
    const u8* p = (u8*)buffer;
    while (length--)
    {
        crc ^= *p++;
        for (int i = 0; i < 8; i++)
        {
            crc = (crc >> 1) ^ ((crc & 1) ? CRCPOLY : 0);
        }
    }

    return crc;
}

CheatsViewModel::CheatsViewModel(const FileInfo& romFileInfo, IRomBrowserController* romBrowserController)
    : _romFileInfo(romFileInfo), _romBrowserController(romBrowserController)
{
    _loadCheatsTask = _romBrowserController->GetIoTaskQueue()->Enqueue([this] (const vu8& cancelRequested)
    {
        LOG_DEBUG("%s\n", _romFileInfo.GetFileName());
        auto file = std::make_unique<File>();
        file->Open(_romFileInfo.GetFastFileRef(), FA_READ);
        auto headerBuffer = std::make_unique_for_overwrite<u8[]>(512);
        if (!file->ReadExact(headerBuffer.get(), 512))
        {
            LOG_ERROR("Could not read rom header\n");
            return TaskResult<void>::Failed();
        }
        file->Close();

        if (cancelRequested)
        {
            return TaskResult<void>::Canceled();
        }

        u32 gameCode = *(u32*)(headerBuffer.get() + 0xC);
        u32 crc = crc32(headerBuffer.get(), 512);
        headerBuffer.reset();

        if (cancelRequested)
        {
            return TaskResult<void>::Canceled();
        }

        _cheats = _romBrowserController->GetCheatRepository().GetCheatsForGame(gameCode, crc);
        if (_cheats)
        {
            _state = State::DisplayCheats;
        }
        else
        {
            _state = State::NoCheats;
        }

        return TaskResult<void>::Completed();
    });
}
