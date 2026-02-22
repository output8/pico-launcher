#include "common.h"
#include "themes/material/MaterialColorScheme.h"
#include "themes/IFontRepository.h"
#include "gui/palette/GradientPalette.h"
#include "gui/GraphicsContext.h"
#include "gui/OamBuilder.h"
#include "CheatListItemView.h"

#define ICON_X             20
#define ICON_Y             3

#define NAME_LABEL_X       40
#define NAME_LABEL_Y       4

CheatListItemView::CheatListItemView(const MaterialColorScheme* materialColorScheme, const IFontRepository* fontRepository)
    : _nameLabel(200, 16, 64, fontRepository->GetFont(FontType::Regular10))
    , _materialColorScheme(materialColorScheme)
{
    _nameLabel.SetEllipsis(true);
    AddChildTail(&_nameLabel);
}

void CheatListItemView::Update()
{
    _nameLabel.SetPosition(NAME_LABEL_X, _position.y + NAME_LABEL_Y);
    ViewContainer::Update();
}

void CheatListItemView::Draw(GraphicsContext& graphicsContext)
{
    auto backColor = _materialColorScheme->GetColor(md::sys::color::surfaceContainerLow);
    _nameLabel.SetBackgroundColor(backColor);
    _nameLabel.SetForegroundColor(_materialColorScheme->onSurface);
    ViewContainer::Draw(graphicsContext);

    if (graphicsContext.IsVisible(Rectangle(_position.x + ICON_X, _position.y + ICON_Y, 16, 16)))
    {
        u32 iconPaletteRow = graphicsContext.GetPaletteManager().AllocRow(
            GradientPalette(backColor, _materialColorScheme->primary),
            _position.y + ICON_Y, _position.y + ICON_Y + 16);
        auto iconOam = graphicsContext.GetOamManager().AllocOams(1);
        OamBuilder::OamWithSize<16, 16>(_position.x + ICON_X, _position.y + ICON_Y, _iconVramOffset >> 7)
            .WithPalette16(iconPaletteRow)
            .WithPriority(graphicsContext.GetPriority())
            .Build(iconOam[0]);
    }
}
