#include "common.h"
#include "gui/input/InputProvider.h"
#include "IconButtonView.h"

bool IconButtonView::HandleInput(const InputProvider& inputProvider, FocusManager& focusManager)
{
    if (inputProvider.Triggered(InputKey::A))
    {
        if (_action && !_disabled)
        {
            _action(this, _actionArg);
        }

        return true;
    }
    return View::HandleInput(inputProvider, focusManager);
}

void IconButtonView::HandlePenDown(const Point& touchPoint, FocusManager& focusManager)
{
    if (!_disabled && GetBounds().Contains(touchPoint))
    {
        _penDown = true;
    }
}

void IconButtonView::HandlePenMove(const Point& touchPoint, FocusManager& focusManager)
{
    if (!GetBounds().Contains(touchPoint))
    {
        _penDown = false;
    }
}

void IconButtonView::HandlePenUp(const Point& lastTouchPoint, FocusManager& focusManager)
{
    if (_penDown && GetBounds().Contains(lastTouchPoint))
    {
        focusManager.Focus(SharedFromThis());

        if (_action)
        {
            _action(this, _actionArg);
        }
    }

    _penDown = false;
}

bool IconButtonView::IsCircleBackgroundVisible() const
{
    switch (_type)
    {
        case Type::Standard:
        {
            return false;
        }
        case Type::Filled:
        case Type::Tonal:
        {
            return true;
        }
        default:
        {
            // shouldn't happen
            return false;
        }
    }
}

md::sys::color IconButtonView::GetCircleBackgroundColor() const
{
    switch (_type)
    {
        case Type::Standard:
        {
            return _backgroundColor;
        }
        case Type::Filled:
        {
            if (_state == State::ToggleUnselected)
                return md::sys::color::surfaceContainerHighest;
            else
                return md::sys::color::primary;
        }
        case Type::Tonal:
        {
            // ToggleActive is only ever set by the favorite button - give it the same
            // prominent look as Filled's "on" state rather than falling into the plain
            // secondaryContainer capsule shared by NoToggle/ToggleSelected, so favoriting
            // reads as active rather than just "not muted".
            if (_state == State::ToggleUnselected)
                return md::sys::color::surfaceContainerHighest;
            else if (_state == State::ToggleActive)
                return md::sys::color::primary;
            else
                return md::sys::color::secondaryContainer;
        }
        default:
        {
            // shouldn't happen
            return md::sys::color::onSurfaceVariant;
        }
    }
}

md::sys::color IconButtonView::GetForegroundColor() const
{
    if (_disabled)
    {
        return md::sys::color::outline;
    }

    switch (_type)
    {
        case Type::Standard:
        {
            // Only the favorite button uses ToggleActive/ToggleSelected on this type (its
            // "active" state is mapped to ToggleActive - see RomBrowserAppBarView::Update()),
            // so both need the accent color for the favorited/not-favorited distinction to show.
            if (_state == State::ToggleSelected || _state == State::ToggleActive)
                return md::sys::color::primary;
            else
                return md::sys::color::onSurfaceVariant;
        }
        case Type::Filled:
        {
            if (_state == State::ToggleUnselected)
                return md::sys::color::primary;
            else
                return md::sys::color::onPrimary;
        }
        case Type::Tonal:
        {
            if (_state == State::ToggleUnselected)
                return md::sys::color::onSurfaceVariant;
            else if (_state == State::ToggleActive)
                return md::sys::color::onPrimary;
            else
                return md::sys::color::onSecondaryContainer;
        }
        default:
        {
            // shouldn't happen
            return md::sys::color::onSurfaceVariant;
        }
    }
}
