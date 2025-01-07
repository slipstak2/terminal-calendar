#include "ListDataSet.h"

ListDataSet::ListDataSet(std::vector<Utf8String> data)
    : items(std::move(data))
{}

const Utf8String& ListDataSet::operator[] (size_t index) const {
    if (IsValidIndex(index)) {
        return items[index];
    }
    return Utf8String::Empty;
}

bool ListDataSet::IsValidIndex(size_t index) const {
    return 0 <= index && index < items.size();
}

bool ListDataSet::Empty() const {
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


std::vector<Utf8String> months{ "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль" , "Август" , "Сентябрь" , "Октябрь" , "Ноябрь" , "Декабрь" };
ListDataSetPtr monthsDataSet = ListDataSet::Create(months);