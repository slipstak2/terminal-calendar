#include "ListDataProvider.h"
#include <algorithm>

ListDataProvider::ListDataProvider(std::vector<Utf8String> items)
    : items(std::move(items))
    , pos(0)
{}

const Utf8String& ListDataProvider::Get() const {
    return items[pos];
}

bool ListDataProvider::Next() {
    if (!HasNext()) {
        return false;
    }
    ++pos;
    OnChange(items[pos-1], items[pos]);
    return true;
}

bool ListDataProvider::HasNext() {
    if (pos == -1) {
        return false;
    }
    return pos + 1 < items.size();
}

bool ListDataProvider::Prev() {
    if (!HasPrev()) {
        return false;
    }
    --pos;
    OnChange(items[pos + 1], items[pos]);
    return true;
}

bool ListDataProvider::HasPrev() {
    if (pos == -1) {
        return false;
    }
    return pos > 0;
}

bool ListDataProvider::Empty() const {
    return items.empty();
}

short ListDataProvider::MaxLen() const {
    if (items.empty()) {
        return 0;
    }

    return (short)std::max_element(items.begin(), items.end(),
        [](const Utf8String& lhs, const Utf8String& rhs) {
            return lhs.size() < rhs.size();
        })->size();
}

void ListDataProvider::OnChange(const Utf8String& prev, const Utf8String& current) {
    for (auto& callback : changeCallbacks) {
        callback(prev, current);
    }
}

void ListDataProvider::AddChangeCallback(ChangeCallback changeCallback) {
    changeCallbacks.push_back(std::move(changeCallback));
}