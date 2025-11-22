#include "common.h"
#include <memory>
#include <libtwl/mem/memVram.h>
#include <libtwl/gfx/gfx.h>
#include <libtwl/gfx/gfxBackground.h>
#include "json/ArduinoJson.h"
#include "core/mini-printf.h"
#include "CustomTopBackgroundType.h"
#include "gui/VramContext.h"
#include "../material/MaterialColorSchemeFactory.h"
#include "romBrowser/views/IconButton3DView.h"
#include "CustomTheme.h"

#define JSON_RESERVED_SIZE  2048

static CustomTopBackgroundType parseTopBackgroundType(const char* topBackgroundTypeString)
{
    return CustomTopBackgroundType::Bitmap;
}

void CustomTheme::LoadRomBrowserResources(const VramContext& mainVramContext, const VramContext& subVramContext)
{
    const auto file = std::make_unique<File>();
    OpenThemeFile(*file, "theme.json");

    u32 fileSize = file->GetSize();
    if (fileSize == 0)
        return;

    std::unique_ptr<u8[]> fileData(new(cache_align) u8[fileSize]);
    u8* fileDataPtr = fileData.get();

    u32 bytesRead = 0;
    if (file->Read(fileDataPtr, fileSize, bytesRead) != FR_OK)
        return;

    DynamicJsonDocument json(JSON_RESERVED_SIZE);
    if (deserializeJson(json, fileDataPtr, fileSize) != DeserializationError::Ok)
        return;

    _topBackgroundType = parseTopBackgroundType(json["topBackgroundType"].as<const char*>());

    mem_setVramDMapping(MEM_VRAM_D_LCDC);
    mem_setVramEMapping(MEM_VRAM_E_LCDC);
    IconButton3DView::UploadGraphics(mainVramContext);
    mem_setVramDMapping(MEM_VRAM_D_TEX_SLOT_0);
    mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
    _romBrowserViewFactory.LoadResources(*this, mainVramContext);
}
