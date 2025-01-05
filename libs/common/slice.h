#pragma once

#include <deque>
#include "utf8string.h"

template<typename T, typename ContainerT = std::deque<T>>
class slice {
public:
    slice(ContainerT& container, int offset, int length) 
        : container(container)
        , offset(offset)
        , length(length)
    {}

    const Utf8String& operator [] (size_t index) const {
        if (index >= length) {
            return Utf8String::Empty;
        }
        if (offset + index >= container.size()) {
            return Utf8String::Empty;
        }
        return container[offset + index];
    }

    size_t size() {
        return length;
    }

protected:
    ContainerT& container;
    int offset = 0;
    int length = 0;
};
