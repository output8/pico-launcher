#pragma once
#include "GameCheats.h"

class ICheatRepository
{
public:
    virtual ~ICheatRepository() { }

    virtual std::unique_ptr<GameCheats> GetCheatsForGame(u32 gameCode, u32 headerCrc32) const = 0;

protected:
    ICheatRepository() { }
};
