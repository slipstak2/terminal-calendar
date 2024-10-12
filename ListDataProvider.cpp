#include "ListDataProvider.h"
#include <algorithm>

ListDataProvider::ListDataProvider(std::vector<Utf8String> data)
    : data(std::move(data))
    , pos(0)
{}

const Utf8String& ListDataProvider::Get() const {
    return data[pos];
}

bool ListDataProvider::Next() {
    if (!HasNext()) {
        return false;
    }
    ++pos;
    OnChange(data[pos-1], data[pos]);
    return true;
}

bool ListDataProvider::HasNext() {
    if (pos == -1) {
        return false;
    }
    return pos + 1 < data.size();
}

bool ListDataProvider::Prev() {
    if (!HasPrev()) {
        return false;
    }
    --pos;
    OnChange(data[pos + 1], data[pos]);
    return true;
}

bool ListDataProvider::HasPrev() {
    if (pos == -1) {
        return false;
    }
    return pos > 0;
}

bool ListDataProvider::Empty() const {
    return data.empty();
}

short ListDataProvider::MaxLen() const {
    if (data.empty()) {
        return 0;
    }

    return (short)std::max_element(data.begin(), data.end(),
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