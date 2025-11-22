#pragma once
#include "SdFolderSortType.h"
#include "SdFolderSortDirection.h"

class SdFolderFilterSortParams
{
public:
    SdFolderSortType sortType = SdFolderSortType::Name;
    SdFolderSortDirection sortDirection = SdFolderSortDirection::Ascending;

    SdFolderFilterSortParams() { }

    SdFolderFilterSortParams(SdFolderSortType sortType, SdFolderSortDirection sortDirection)
        : sortType(sortType), sortDirection(sortDirection) { }
};
