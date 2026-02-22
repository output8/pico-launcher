#pragma once
#include "Cheat.h"
#include "CheatCategory.h"

class GameCheats
{
public:
    GameCheats(std::unique_ptr<u8[]> cheatData, const char* gameName,
        CheatCategory* categories, u32 numberOfCategories, Cheat* cheats, u32 numberOfCheats)
        : _cheatData(std::move(cheatData)), _gameName(gameName)
        , _categories(categories), _numberOfCategories(numberOfCategories)
        , _cheats(cheats), _numberOfCheats(numberOfCheats) { }

    ~GameCheats()
    {
        if (_categories != nullptr)
        {
            free(_categories);
        }
        if (_cheats != nullptr)
        {
            free(_cheats);
        }
    }

    const char* GetGameName() const
    {
        return _gameName;
    }

    const CheatCategory* GetCheatCategories(u32& numberOfCategories) const
    {
        numberOfCategories = _numberOfCategories;
        return _categories;
    }

    const Cheat* GetCheats(u32& numberOfCheats) const
    {
        numberOfCheats = _numberOfCheats;
        return _cheats;
    }

private:
    std::unique_ptr<u8[]> _cheatData;
    const char* _gameName;
    CheatCategory* _categories;
    u32 _numberOfCategories;
    Cheat* _cheats;
    u32 _numberOfCheats;
};
