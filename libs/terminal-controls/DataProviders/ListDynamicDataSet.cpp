#include "ListDynamicDataSet.h"

void ListDynamicDataSet::AddItem(const Utf8String& item) {
    items.push_back(item);
    OnChangeItemsCount(items.size(), items.size() - 1);
}

bool ListDynamicDataSet::RemoveLastItem() {
    if (!items.empty()) {
        items.pop_back();
        OnChangeItemsCount(items.size(), items.size() + 1);
        return true;
    }
    return false;
}

int ListDynamicDataSet::TotalItems() const {
    return (int)items.size();
}

slice<Utf8String> ListDynamicDataSet::GetView(int offset, int length) {
    length = std::min(length, (int)items.size());
    return slice<Utf8String>(items, offset, length);
}


void ListDynamicDataSet::OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount)
{
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, curItemsCount, prvItemsCount);
    }
}

