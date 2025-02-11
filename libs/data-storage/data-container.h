#pragma once
#include <string_view>

template<typename T>
class DataContainer : public std::enable_shared_from_this<T> {
public:
    virtual size_t RowsCount() const = 0;

    size_t GetFieldIndex(size_t field_index) const {
        return field_index;
    }
    virtual size_t GetFieldIndex(const std::string_view field_name) const = 0;
};
