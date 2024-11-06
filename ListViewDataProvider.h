#pragma once

#include "utils/utf8string.h"
#include "utils/slice.h"
#include <deque>
#include <functional>

class ListViewDataProvider;
using ListViewDataProviderChangedItemsCountCallback = std::function<void(const ListViewDataProvider* provider, int curItemsCount, int prvItemsCount)>;

class ListViewDataProvider {
public:
    DECLARE_CREATE(ListViewDataProvider)

    ListViewDataProvider() = default;

    void AddItem(const Utf8String& item);
    bool RemoveLastItem();

    int TotalItems() const;

    slice<Utf8String> GetView(int offset, int length);

public:
    void AddChangeItemsCallback(ListViewDataProviderChangedItemsCountCallback changeItemsCountCallback) {
        changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
    }
protected:
    void OnChangeItemsCount(int curItemsCount, int prvItemsCount);

protected:
    std::deque<Utf8String> items;
    std::vector<ListViewDataProviderChangedItemsCountCallback> changeItemsCountCallbacks;
};