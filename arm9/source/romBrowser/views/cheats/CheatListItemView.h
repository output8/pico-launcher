#pragma once
#include "gui/views/ViewContainer.h"
#include "gui/views/Label2DView.h"

class MaterialColorScheme;
class IFontRepository;

class CheatListItemView : public ViewContainer
{
public:
    CheatListItemView(const MaterialColorScheme* materialColorScheme, const IFontRepository* fontRepository);

    void Update() override;
    void Draw(GraphicsContext& graphicsContext) override;

    Rectangle GetBounds() const override
    {
        return Rectangle(_position.x, _position.y, 256, 24);
    }

    void SetName(const char* name)
    {
        _nameLabel.SetText(name);
    }

    void SetIcon(u32 iconVramOffset)
    {
        _iconVramOffset = iconVramOffset;
    }

private:
    Label2DView _nameLabel;
    u32 _iconVramOffset = 0;
    const MaterialColorScheme* _materialColorScheme;
};
