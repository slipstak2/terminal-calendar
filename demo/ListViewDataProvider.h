#pragma once

#include "common.h"
#include "utf8string.h"
#include "slice.h"

#include <deque>
#include <functional>

class ListViewDataProvider;
using ListViewDataProviderChangedItemsCountCallback = std::function<void(const ListViewDataProvider* provider, size_t curItemsCount, size_t prvItemsCount)>;

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
    void OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount);

protected:
    std::deque<Utf8String> items;
    std::vector<ListViewDataProviderChangedItemsCountCallback> changeItemsCountCallbacks;
};