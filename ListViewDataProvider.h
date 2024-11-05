#pragma once

#include "utils/utf8string.h"
#include "utils/slice.h"
#include <deque>

class ListViewDataProvider {
public:
    DECLARE_CREATE(ListViewDataProvider)

    ListViewDataProvider() = default;

    void AddItem(const Utf8String& item);

    int TotalItems() const;

    slice<Utf8String> GetView(int offset, int length);

protected:
    std::deque<Utf8String> items;
};