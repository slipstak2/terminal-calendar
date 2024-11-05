#include "ListViewDataProvider.h"

void ListViewDataProvider::AddItem(const Utf8String& item) {
    items.push_back(item);
}

int ListViewDataProvider::TotalItems() const {
    return items.size();
}

slice<Utf8String> ListViewDataProvider::GetView(int offset, int length) {
    //offset = std::max(0, (int)items.size() - length);
    length = std::min(length, (int)items.size());
    return slice<Utf8String>(items, offset, length);
}