#pragma once
#include "gui/views/RecyclerAdapter.h"
#include "cheats/CheatCategory.h"
#include "cheats/Cheat.h"
#include "CheatListItemView.h"

class CheatsAdapter : public RecyclerAdapter
{
public:
    CheatsAdapter(const CheatCategory* categories, u32 numberOfCategories, const Cheat* cheats, u32 numberOfCheats,
        const MaterialColorScheme* materialColorScheme, const IFontRepository* fontRepository,
        u32 folderIconVramOffset, u32 checkboxUncheckedIconVramOffset, u32 checkboxCheckedIconVramOffset)
        : _categories(categories), _numberOfCategories(numberOfCategories), _cheats(cheats), _numberOfCheats(numberOfCheats)
        , _materialColorScheme(materialColorScheme), _fontRepository(fontRepository)
        , _folderIconVramOffset(folderIconVramOffset)
        , _checkboxUncheckedIconVramOffset(checkboxUncheckedIconVramOffset)
        , _checkboxCheckedIconVramOffset(checkboxCheckedIconVramOffset) { }

    u32 GetItemCount() const override
    {
        return _numberOfCategories + _numberOfCheats;
    }

    void GetViewSize(int& width, int& height) const override
    {
        width = 256;
        height = 24;
    }

    View* CreateView() const override
    {
        LOG_DEBUG("CheatsAdapter::CreateView\n");
        return new CheatListItemView(_materialColorScheme, _fontRepository);
    }

    void DestroyView(View* view) const override
    {
        LOG_DEBUG("CheatsAdapter::DestroyView\n");
        delete (CheatListItemView*)view;
    }

    void BindView(View* view, int index) const override
    {
        LOG_DEBUG("CheatsAdapter::BindView\n");
        auto listItemView = static_cast<CheatListItemView*>(view);
        if ((u32)index < _numberOfCategories)
        {
            listItemView->SetName(_categories[index].GetName());
            listItemView->SetIcon(_folderIconVramOffset);
        }
        else
        {
            index -= _numberOfCategories;
            listItemView->SetName(_cheats[index].GetName());
            listItemView->SetIcon(_cheats[index].GetIsCheatActive() ? _checkboxCheckedIconVramOffset : _checkboxUncheckedIconVramOffset);
        }
    }

    void ReleaseView(View* view, int index) const override
    {
        LOG_DEBUG("CheatsAdapter::ReleaseView\n");
    }

private:
    const CheatCategory* _categories;
    u32 _numberOfCategories;
    const Cheat* _cheats;
    u32 _numberOfCheats;
    const MaterialColorScheme* _materialColorScheme;
    const IFontRepository* _fontRepository;
    u32 _folderIconVramOffset;
    u32 _checkboxUncheckedIconVramOffset;
    u32 _checkboxCheckedIconVramOffset;
};
