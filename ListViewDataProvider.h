#pragma once

#include "utils/utf8string.h"
#include "utils/slice.h"
#include <deque>

class ListViewDataProvider {
public:
    DECLARE_CREATE(ListViewDataProvider)

    ListViewDataProvider() = default;

    void AddItem(const Utf8String& item);

    slice<Utf8String> GetLast(int length);

protected:
    std::deque<Utf8String> items;
};