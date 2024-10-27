#include "ListViewDataProvider.h"

void ListViewDataProvider::AddItem(const Utf8String& item) {
    items.push_back(item);
}

slice<Utf8String> ListViewDataProvider::GetLast(int length) {
    int offset = std::max(0, (int)items.size() - length);
    length = std::min(length, (int)items.size());
    return slice<Utf8String>(items, offset, length);
}