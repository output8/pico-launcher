#pragma once
#include "CheatTreeItem.h"

class Cheat : public CheatTreeItem
{
public:
    Cheat()
        : _cheatData(nullptr), _cheatDataLength(0) { }

    Cheat(const char* name, const char* description, bool isCheatActive, const void* cheatData, u32 cheatDataLength)
        : CheatTreeItem(name, description), _isCheatActive(isCheatActive)
        , _cheatData(cheatData), _cheatDataLength(cheatDataLength) { }

    bool GetIsCheatActive() const
    {
        return _isCheatActive;
    }

    void SetIsCheatActive(bool isCheatActive)
    {
        _isCheatActive = isCheatActive;
    }

private:
    bool _isCheatActive;
    const void* _cheatData;
    u32 _cheatDataLength;
};
