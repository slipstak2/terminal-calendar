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


std::vector<size_t> GenRowsNum(size_t offset, size_t rows_count);
std::vector<size_t> GenFieldsNum(size_t fields_count);

class DataContainer {
public:
    virtual size_t RowsCount() const = 0;

    size_t GetFieldIndex(size_t field_index) const {
        return field_index;
    }

    std::string ToString();

    virtual size_t GetFieldIndex(const std::string_view field_name) const = 0;

    virtual size_t FieldsCount() const = 0;

    virtual std::string_view GetFieldName(size_t field_num) const = 0;

    virtual DataFieldAccessorPtr GetRow(size_t row_num) = 0;

    VIRTUAL_DECLARATIONS_VIEW;
    
    virtual DataContainerPtr SelfPtr() = 0;

    virtual DataSetPtr AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) = 0;
    
    virtual DataViewPtr Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) = 0;
    
    virtual DataViewPtr Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) = 0;
    
    template<typename T>
    DataViewPtr Unique();

    template<typename T>
    DataViewPtr Unique(size_t fn0);

    template<typename T>
    DataViewPtr Unique(const std::string_view fn0);

    DataViewPtr Limit(const size_t limit, const size_t offset = 0);
};


template<typename T>
DataViewPtr DataContainer::Unique(size_t fn0) {
    std::unordered_set<T> mem;
    std::vector<size_t> rows_num_unique;
    rows_num_unique.reserve(RowsCount());
    for (size_t row_num = 0; row_num < RowsCount(); ++row_num) {
        auto row = GetRow(row_num);
        T val = row->GetField<T>(fn0);
        if (!mem.contains(val)) {
            mem.insert(val);
            rows_num_unique.push_back(row_num);
        }
    }
    rows_num_unique.shrink_to_fit();
    return DataView::Create(SelfPtr(), GenFieldsNum(FieldsCount()), rows_num_unique);
}


template<typename T>
DataViewPtr DataContainer::Unique(const std::string_view fn0) {
    return Unique<T>(GetFieldIndex(fn0));
}

template<typename T>
DataViewPtr DataContainer::Unique() {
    return Unique<T>(0);
}