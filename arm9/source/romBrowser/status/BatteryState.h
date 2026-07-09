#pragma once
#include "statusPayload.h"

enum class BatteryIcon { Empty, Low, Medium, Charged, Full, Charging };

inline bool IsWarning(BatteryIcon i)
{
    return i == BatteryIcon::Low || i == BatteryIcon::Empty;
}

inline BatteryIcon ResolveBatteryIcon(const StatusPayload& s)
{
    if (s.flags & STATUS_FLAG_CHARGING) return BatteryIcon::Charging; // bolt overrides, even at full

    if (s.flags & STATUS_FLAG_HAS_FINE_LEVEL)
    {
        // The DSi reports icon step markers in bits 0-3 of BPTWL reg 0x20: 0xF=full, 0xB=3 bars,
        // 0x7=2 bars, 0x3=1 solid red bar, 0x1=1 blinking red bar, 0x0=critical/shutdown.
        // Match the native menu by reserving the Full icon for 0xF itself rather than rounding
        // 0xD/0xE up to Full. The level is only meaningful when not charging - the charging branch
        // above already returns before we get here.
        u8 level = s.batteryLevel;
        if (level >= 0x0F) return BatteryIcon::Full;     // 0xF
        if (level >= 0x0B) return BatteryIcon::Charged;  // 0xB..0xE
        if (level >= 0x07) return BatteryIcon::Medium;   // 0x7..0xA
        if (level >= 0x01) return BatteryIcon::Low;      // 0x1..0x6
        return BatteryIcon::Empty;                        // 0x0
    }
    // DS / DS-Lite binary: 1 = low
    return s.batteryLevel ? BatteryIcon::Low : BatteryIcon::Full;
}
