#pragma once

#include "ListDataSetBase.h"
#include "common.h"

#include <vector>

class ListDataSet : public ListDataSetBase {
public:
    DECLARE_CREATE(ListDataSet);

    explicit ListDataSet(std::vector<Utf8String> items);
    const Utf8String& operator[] (size_t index) const override;
    bool IsValidIndex(size_t index) const override;
    bool Empty() const override;
    short MaxLen() const override;

protected:
    std::vector<Utf8String> items;
};

extern ListDataSetPtr monthsDataSet;