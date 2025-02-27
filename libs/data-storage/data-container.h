#pragma once
#include "defines.h"
#include "common.h"
#include "data-field.h"
#include "data-field-accessor.h"

#include <string_view>
#include <functional>
#include <numeric>
#include <vector>
#include <unordered_set>


std::vector<size_t> GenFieldsNum(size_t fields_count);

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
    
    VIRTUAL_DECLARATIONS_UNIQUE;
};


template<typename T>
DataViewPtr Unique(const DataContainerPtr& container, const std::string_view fn0) {
    std::unordered_set<T> mem;
    std::vector<size_t> rows_num_unique;
    rows_num_unique.reserve(container->RowsCount());
    for (size_t row_num = 0; row_num < container->RowsCount(); ++row_num) {
        auto row = container->GetRow(row_num);
        T val = row->GetField<T>(fn0);
        if (!mem.contains(val)) {
            mem.insert(val);
            rows_num_unique.push_back(row_num);
        }
    }
    rows_num_unique.shrink_to_fit();
    return DataView::Create(container, GenFieldsNum(container->FieldsCount()), rows_num_unique);
}
