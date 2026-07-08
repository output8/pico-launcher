#pragma once
#include "common.h"
#include "gui/VramContext.h"
#include "gui/GraphicsContext.h"
#include "gui/OamBuilder.h"
#include "gui/palette/GradientPalette.h"
#include "core/math/Rgb.h"
#include "favoriteBadge.h"

class FavoriteBadgeVramToken
{
    u32 _vramOffset;
public:
    FavoriteBadgeVramToken()
        : _vramOffset(0) { }

    explicit FavoriteBadgeVramToken(u32 offset)
        : _vramOffset(offset) { }

    constexpr u32 GetVramOffset() const { return _vramOffset; }
};

namespace FavoriteBadge
{
    inline FavoriteBadgeVramToken UploadGraphics(const VramContext& vramContext)
    {
        const auto objVramManager = vramContext.GetObjVramManager();
        u32 vramOffset = 0;
        if (objVramManager)
        {
            vramOffset = objVramManager->Alloc(favoriteBadgeTilesLen);
            dma_ntrCopy32(3, favoriteBadgeTiles, objVramManager->GetVramAddress(vramOffset), favoriteBadgeTilesLen);
        }
        return FavoriteBadgeVramToken(vramOffset);
    }

    inline void Draw(GraphicsContext& graphicsContext, int badgeX, int badgeY, u32 favoriteBadgeVramOffset)
    {
        u32 paletteRow = graphicsContext.GetPaletteManager().AllocRow(
            GradientPalette(Rgb<8, 8, 8>(255, 255, 255), Rgb<8, 8, 8>(229, 57, 53)),
            badgeY, badgeY + 16);

        gfx_oam_entry_t* oam = graphicsContext.GetOamManager().AllocOams(1);
        OamBuilder::OamWithSize<16, 16>(badgeX, badgeY, favoriteBadgeVramOffset >> 7)
            .WithPalette16(paletteRow)
            .WithPriority(graphicsContext.GetPriority())
            .Build(oam[0]);
    }
}
