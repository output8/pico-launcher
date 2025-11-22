#pragma once
#include "../../views/IconGridItemView.h"

class CustomIconGridItemView : public IconGridItemView
{
public:
    CustomIconGridItemView(u32 texVramOffset, u32 plttVramOffset,
        u32 selectedTexVramOffset, u32 selectedPlttVramOffset)
        : _texVramOffset(texVramOffset), _plttVramOffset(plttVramOffset)
        , _selectedTexVramOffset(selectedTexVramOffset), _selectedPlttVramOffset(selectedPlttVramOffset) { }

    void Draw(GraphicsContext& graphicsContext) override;

    Rectangle GetBounds() const override
    {
        return Rectangle(_position.x + 2, _position.y + 2, 40, 40);
    }

private:
    u32 _texVramOffset = 0;
    u32 _plttVramOffset = 0;
    u32 _selectedTexVramOffset = 0;
    u32 _selectedPlttVramOffset = 0;
};