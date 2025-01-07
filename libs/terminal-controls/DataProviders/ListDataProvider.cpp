#include "ListDataProvider.h"
#include <algorithm>

ListDataProvider::ListDataProvider(ListDataSetBasePtr dataSet)
    : dataSet(dataSet)
    , pos(0)
{}

ListDataProvider::ListDataProvider(ListDataSetBasePtr dataSet, int pos)
    : dataSet(dataSet)
    , pos(pos == -1 ? 0 : pos)
{}

const Utf8String& ListDataProvider::Get() const {
    return (*dataSet)[pos];
}

bool ListDataProvider::Next() {
    if (!HasNext()) {
        return false;
    }
    ++pos;
    OnChange((*dataSet)[pos-1], (*dataSet)[pos]);
    return true;
}

bool ListDataProvider::HasNext() {
    if (pos == -1) {
        return false;
    }
    return dataSet->IsValidIndex(pos + 1);
}

bool ListDataProvider::Prev() {
    if (!HasPrev()) {
        return false;
    }
    --pos;
    OnChange((*dataSet)[pos + 1], (*dataSet)[pos]);
    return true;
}

bool ListDataProvider::HasPrev() {
    if (pos == -1) {
        return false;
    }
    return pos > 0;
}

bool ListDataProvider::Empty() const {
    return dataSet->Empty();
}

short ListDataProvider::MaxLen() const {
    return dataSet->MaxLen();
}

void ListDataProvider::OnChange(const Utf8String& prev, const Utf8String& current) {
    for (auto& callback : changeCallbacks) {
        callback(prev, current);
    }
}

void ListDataProvider::AddChangeCallback(ChangeCallback changeCallback) {
    changeCallbacks.push_back(std::move(changeCallback));
}