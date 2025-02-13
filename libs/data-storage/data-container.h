#pragma once
#include "defines.h"

#include <string_view>
#include <functional>

class DataContainer {
public:
    virtual size_t RowsCount() const = 0;

    size_t GetFieldIndex(size_t field_index) const {
        return field_index;
    }
    virtual size_t GetFieldIndex(const std::string_view field_name) const = 0;

    virtual size_t FieldsCount() const = 0;

    virtual std::string_view GetFieldName(size_t field_num) const = 0;

    virtual DataFieldAccessorPtr GetRow(size_t row_num) = 0;

    VIRTUAL_DECLARATIONS_VIEW;
    
    virtual DataContainerPtr AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& cb) = 0;
};
