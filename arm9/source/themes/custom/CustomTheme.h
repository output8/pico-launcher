#pragma once
#include "fat/ff.h"
#include "fat/File.h"
#include "core/String.h"
#include "CustomMainBackground.h"
#include "CustomSubBackground.h"
#include "romBrowser/Theme/custom/CustomRomBrowserViewFactory.h"
#include "CustomTopBackgroundType.h"
#include "../DefaultFontRepository.h"
#include "../Theme.h"

class CustomTheme : public Theme
{
    String<TCHAR, 64> _folderName;
    CustomRomBrowserViewFactory _romBrowserViewFactory;
    CustomTopBackgroundType _topBackgroundType;
    DefaultFontRepository _fontRepository;

public:
    CustomTheme(const TCHAR* folderName, const Rgb<8, 8, 8>& primaryColor, bool darkMode)
        : Theme(folderName, primaryColor, darkMode)
        , _romBrowserViewFactory(&_materialColorScheme, &_fontRepository) { }

    const IFontRepository* GetFontRepository() const override
    {
        return &_fontRepository;
    }

    const IThemeFileIconFactory* GetThemeFileIconFactory() const override
    {
        return nullptr;
    }

    const IRomBrowserViewFactory* GetRomBrowserViewFactory() const override
    {
        return &_romBrowserViewFactory;
    }

    std::unique_ptr<IThemeBackground> CreateRomBrowserTopBackground() const override
    {
        return std::make_unique<CustomSubBackground>();
    }

    std::unique_ptr<IThemeBackground> CreateRomBrowserBottomBackground() const override
    {
        return std::make_unique<CustomMainBackground>();
    }

    void LoadRomBrowserResources(const VramContext& mainVramContext, const VramContext& subVramContext) override;
};
