#pragma once

#include "utf8string.h"

class ListDataSetBase {
public:
    virtual const Utf8String& operator[] (size_t index) const = 0;
    virtual bool IsValidIndex(size_t index) const = 0;
    virtual bool Empty() const = 0;
    virtual short MaxLen() const = 0;
};