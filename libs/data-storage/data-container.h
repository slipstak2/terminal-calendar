#pragma once
#include "defines.h"
#include "common.h"
#include "data-field.h"

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
    
    virtual DataContainerPtr AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) = 0;
    
    virtual DataContainerPtr Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) = 0;
    
    virtual DataContainerPtr Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) = 0;
};
