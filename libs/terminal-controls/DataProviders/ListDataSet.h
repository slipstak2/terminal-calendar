#pragma once

#include <vector>
#include "Utf8String.h"
#include "common.h"

class ListDataSet {
public:
    DECLARE_CREATE(ListDataSet);

    explicit ListDataSet(std::vector<Utf8String> items);
    const Utf8String& operator[] (size_t index) const;
    size_t size() const;
    bool empty() const;
    short MaxLen() const;

protected:
    std::vector<Utf8String> items;
};