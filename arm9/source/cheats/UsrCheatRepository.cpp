#include "common.h"
#include <algorithm>
#include <string.h>
#include "UsrCheatRepository.h"

std::unique_ptr<GameCheats> UsrCheatRepository::GetCheatsForGame(u32 gameCode, u32 headerCrc32) const
{
    auto index = FindIndex(gameCode, headerCrc32);
    if (index == nullptr)
    {
        LOG_DEBUG("Cheats not found for %c%c%c%c - 0x%X\n",
            gameCode & 0xFF, (gameCode >> 8) & 0xFF, (gameCode >> 16) & 0xFF, gameCode >> 24,
            headerCrc32);
        return nullptr;
    }

    auto cheatData = std::make_unique_for_overwrite<u8[]>(index->padding); // padding was set to the size in UsrCheatRepositoryFactory
    if (_usrCheatFile->Seek(index->offset) != FR_OK)
    {
        LOG_ERROR("Failed to seek to cheat data\n");
        return nullptr;
    }
    if (!_usrCheatFile->ReadExact(cheatData.get(), index->padding))
    {
        LOG_ERROR("Failed to read cheat data\n");
        return nullptr;
    }

    u8* ptr = cheatData.get();

    // game name
    const char* gameName = (const char*)ptr;
    ptr += strlen(gameName) + 1;

    // padding
    ptr = (u8*)(((u32)ptr + 3) & ~3); // 32-bit align

    // flags
    u32 flags = *(u32*)ptr;
    u32 totalNumberOfItems = flags & 0x0FFFFFFF;
    u32 gameActive = flags >> 28;
    ptr += 4;

    // master codes
    ptr += 8 * 4;

    auto categories = (CheatCategory*)malloc(totalNumberOfItems * sizeof(CheatCategory));
    u32 categoryCount = 0;

    auto cheats = (Cheat*)malloc(totalNumberOfItems * sizeof(Cheat));
    u32 cheatCount = 0;

    while (ptr < cheatData.get() + index->padding)
    {
        u32 itemFlags = *(u32*)ptr;
        bool isCategory = ((itemFlags >> 28) & 1) == 1;
        if (isCategory)
        {
            ParseCategory(categories[categoryCount], ptr);
            categoryCount++;
        }
        else
        {
            ParseCheat(cheats[cheatCount], ptr);
            cheatCount++;
        }
    }

    categories = (CheatCategory*)realloc(categories, categoryCount * sizeof(CheatCategory));
    cheats = (Cheat*)realloc(cheats, cheatCount * sizeof(Cheat));

    return std::make_unique<GameCheats>(std::move(cheatData), gameName, categories, categoryCount, cheats, cheatCount);
}

const usr_cheat_index_entry_t* UsrCheatRepository::FindIndex(u32 gameCode, u32 headerCrc32) const
{
    if (_numberOfIndices != 0)
    {
        const auto index = std::lower_bound(_sortedIndices.get(), _sortedIndices.get() + _numberOfIndices, gameCode,
            [headerCrc32] (const usr_cheat_index_entry_t& entry, u32 value)
            {
                if (entry.gameCode != value)
                {
                    return entry.gameCode < value;
                }

                return entry.headerCrc32 < headerCrc32;
            });

        if (index != _sortedIndices.get() + _numberOfIndices &&
            index->gameCode == gameCode &&
            index->headerCrc32 == headerCrc32)
        {
            return index;
        }
    }

    return nullptr;
}

void UsrCheatRepository::ParseCategory(CheatCategory& category, u8*& ptr) const
{
    // flags
    u32 itemFlags = *(u32*)ptr;
    ptr += 4;
    u32 numberOfItems = itemFlags & 0x00FFFFFF;
    bool isMaxOneCheatActive = ((itemFlags >> 24) & 1) == 1;

    // item name
    const char* itemName = (const char*)ptr;
    ptr += strlen(itemName) + 1;

    // item description
    const char* itemDescription = (const char*)ptr;
    ptr += strlen(itemDescription) + 1;

    // padding
    ptr = (u8*)(((u32)ptr + 3) & ~3); // 32-bit align

    auto categories = (CheatCategory*)malloc(numberOfItems * sizeof(CheatCategory));
    u32 categoryCount = 0;

    auto cheats = (Cheat*)malloc(numberOfItems * sizeof(Cheat));
    u32 cheatCount = 0;

    for (u32 i = 0; i < numberOfItems; i++)
    {
        u32 itemFlags = *(u32*)ptr;
        bool isCategory = ((itemFlags >> 28) & 1) == 1;
        if (isCategory)
        {
            ParseCategory(categories[categoryCount], ptr);
            categoryCount++;
        }
        else
        {
            ParseCheat(cheats[cheatCount], ptr);
            cheatCount++;
        }
    }

    categories = (CheatCategory*)realloc(categories, categoryCount * sizeof(CheatCategory));
    cheats = (Cheat*)realloc(cheats, cheatCount * sizeof(Cheat));

    category = CheatCategory(itemName, itemDescription, isMaxOneCheatActive, categories, categoryCount, cheats, cheatCount);
}

void UsrCheatRepository::ParseCheat(Cheat& cheat, u8*& ptr) const
{
    // flags
    u32 itemFlags = *(u32*)ptr;
    ptr += 4;
    bool isCheatActive = ((itemFlags >> 24) & 1) == 1;

    // item name
    const char* itemName = (const char*)ptr;
    ptr += strlen(itemName) + 1;

    // item description
    const char* itemDescription = (const char*)ptr;
    ptr += strlen(itemDescription) + 1;

    // padding
    ptr = (u8*)(((u32)ptr + 3) & ~3); // 32-bit align

    // number of code words
    u32 numberOfCodeWords = *(u32*)ptr;
    ptr += 4;

    cheat = Cheat(itemName, itemDescription, isCheatActive, ptr, numberOfCodeWords * 4);

    // code
    ptr += numberOfCodeWords * 4;
}
