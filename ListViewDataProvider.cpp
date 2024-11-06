#include "ListViewDataProvider.h"

void ListViewDataProvider::AddItem(const Utf8String& item) {
    items.push_back(item);
    OnChangeItemsCount(items.size(), items.size() - 1);
}

bool ListViewDataProvider::RemoveLastItem() {
    if (!items.empty()) {
        items.pop_back();
        OnChangeItemsCount(items.size(), items.size() + 1);
        return true;
    }
    return false;
}

void ListViewDataProvider::OnChangeItemsCount(int curItemsCount, int prvItemsCount)
{
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, curItemsCount, prvItemsCount);
    }
}

int ListViewDataProvider::TotalItems() const {
    return items.size();
}

slice<Utf8String> ListViewDataProvider::GetView(int offset, int length) {
    //offset = std::max(0, (int)items.size() - length);
    length = std::min(length, (int)items.size());
    return slice<Utf8String>(items, offset, length);
}