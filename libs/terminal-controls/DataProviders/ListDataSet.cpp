#include "ListDataSet.h"

ListDataSet::ListDataSet(std::vector<Utf8String> data)
    : items(std::move(data))
{}

const Utf8String& ListDataSet::operator[] (size_t index) const {
    if (0 <= index && index < items.size()) {
        return items[index];
    }
    return Utf8String::Empty;
}

size_t ListDataSet::size() const {
    return items.size();
}

bool ListDataSet::empty() const {
    return items.empty();
}

short ListDataSet::MaxLen() const {
    if (items.empty()) {
        return 0;
    }

    return (short)std::max_element(items.begin(), items.end(),
        [](const Utf8String& lhs, const Utf8String& rhs) {
            return lhs.size() < rhs.size();
        })->size();
}
