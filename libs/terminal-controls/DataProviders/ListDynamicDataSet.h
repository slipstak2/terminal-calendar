#pragma once

#include "slice.h"
#include "Utf8String.h"
#include "common.h"

#include <deque>
#include <functional>

class ListDynamicDataSet;
using ListDynamicDataSetChangedItemsCountCallback = std::function<void(const ListDynamicDataSet* dataSet, size_t prvItemsCount)>;

class ListDynamicDataSet {
public:
    DECLARE_CREATE(ListDynamicDataSet);

    void AddItem(const Utf8String& item);
    bool RemoveLastItem();
    int TotalItems() const;

    slice<Utf8String> GetView(int offset, int length);

public:
    void AddChangeItemsCountCallback(ListDynamicDataSetChangedItemsCountCallback changeItemsCountCallback) {
        changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
    }
protected:
    void OnChangeItemsCount(size_t prvItemsCount);

protected:
    std::deque<Utf8String> items; // deque for GetView
    std::vector<ListDynamicDataSetChangedItemsCountCallback> changeItemsCountCallbacks;
};