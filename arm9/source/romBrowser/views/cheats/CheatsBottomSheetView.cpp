#include "common.h"
#include "gui/GraphicsContext.h"
#include "themes/material/MaterialColorScheme.h"
#include "themes/IFontRepository.h"
#include "gui/input/InputProvider.h"
#include "gui/VramContext.h"
#include "folderIcon.h"
#include "checkboxChecked.h"
#include "checkboxUnchecked.h"
#include "CheatsBottomSheetView.h"

#define TITLE_LABEL_X       20
#define TITLE_LABEL_Y       16

CheatsBottomSheetView::CheatsBottomSheetView(std::unique_ptr<CheatsViewModel> viewModel,
    const MaterialColorScheme* materialColorScheme, const IFontRepository* fontRepository,
    FocusManager* focusManager)
    : _viewModel(std::move(viewModel))
    , _titleLabel(128, 16, 25, fontRepository->GetFont(FontType::Medium11))
    , _cheatListRecycler(0, 36, 256, 124, RecyclerView::Mode::VerticalList)
    , _materialColorScheme(materialColorScheme)
    , _fontRepository(fontRepository)
    , _focusManager(focusManager)
{
    _titleLabel.SetText(u"Cheats");
    AddChildTail(&_titleLabel);
    AddChildTail(&_cheatListRecycler);
}

void CheatsBottomSheetView::InitVram(const VramContext& vramContext)
{
    BottomSheetView::InitVram(vramContext);

    const auto objVramManager = vramContext.GetObjVramManager();
    if (objVramManager)
    {
        _folderIconVramOffset = objVramManager->Alloc(folderIconTilesLen);
        dma_ntrCopy32(3, folderIconTiles,
            objVramManager->GetVramAddress(_folderIconVramOffset), folderIconTilesLen);

        _checkboxUncheckedIconVramOffset = objVramManager->Alloc(checkboxUncheckedTilesLen);
        dma_ntrCopy32(3, checkboxUncheckedTiles,
            objVramManager->GetVramAddress(_checkboxUncheckedIconVramOffset), checkboxUncheckedTilesLen);

        _checkboxCheckedIconVramOffset = objVramManager->Alloc(checkboxCheckedTilesLen);
        dma_ntrCopy32(3, checkboxCheckedTiles,
            objVramManager->GetVramAddress(_checkboxCheckedIconVramOffset), checkboxCheckedTilesLen);
    }

    _objVramManager = vramContext.GetObjVramManager();
}

void CheatsBottomSheetView::Update()
{
    _titleLabel.SetPosition(TITLE_LABEL_X, _position.y + TITLE_LABEL_Y);
    _cheatListRecycler.SetPosition(0, _position.y + 36);
    if (_viewModel->GetState() == CheatsViewModel::State::DisplayCheats)
    {
        if (_cheatsAdapter == nullptr)
        {
            LOG_DEBUG("Setting adapter\n");
            auto gameCheats = _viewModel->GetCheats();
            u32 numberOfCategories = 0;
            auto categories = gameCheats->GetCheatCategories(numberOfCategories);
            u32 numberOfCheats = 0;
            auto cheats = gameCheats->GetCheats(numberOfCheats);
            _cheatsAdapter = new CheatsAdapter(categories, numberOfCategories, cheats, numberOfCheats,
                _materialColorScheme, _fontRepository, _folderIconVramOffset,
                _checkboxUncheckedIconVramOffset, _checkboxCheckedIconVramOffset);
            _cheatListRecycler.SetAdapter(_cheatsAdapter);
            _cheatListRecycler.InitVram(VramContext(nullptr, _objVramManager, nullptr, nullptr));
            _cheatListRecycler.Focus(*_focusManager);
            LOG_DEBUG("Setting adapter done\n");
        }
    }
    BottomSheetView::Update();
}

void CheatsBottomSheetView::Draw(GraphicsContext& graphicsContext)
{
    graphicsContext.SetClipArea(GetBounds());
    u32 oldPrio = graphicsContext.SetPriority(1);
    {
        _titleLabel.SetBackgroundColor(_materialColorScheme->GetColor(md::sys::color::surfaceContainerLow));
        _titleLabel.SetForegroundColor(_materialColorScheme->onSurface);
        _titleLabel.Draw(graphicsContext);
        graphicsContext.SetClipArea(_cheatListRecycler.GetBounds());
        _cheatListRecycler.Draw(graphicsContext);
        // BottomSheetView::Draw(graphicsContext);
    }
    graphicsContext.SetPriority(oldPrio);
    graphicsContext.ResetClipArea();
}

bool CheatsBottomSheetView::HandleInput(const InputProvider& inputProvider, FocusManager& focusManager)
{
    if (inputProvider.Triggered(InputKey::B))
    {
        _viewModel->Close();
        return true;
    }
    return false;
}
