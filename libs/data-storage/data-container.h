#pragma once
#include <string_view>

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

    virtual DataViewPtr View() = 0;
    /*
    virtual DataViewPtr View(size_t fn0) { return nullptr; }
    virtual DataViewPtr View(size_t fn0, size_t fn1) { return nullptr; }
    virtual DataViewPtr View(size_t fn0, size_t fn1, size_t fn2) { return nullptr; }

    virtual DataViewPtr View(const std::string_view fn0) { return nullptr; }
    virtual DataViewPtr View(const std::string_view fn0, const std::string_view fn1) { return nullptr; }
    virtual DataViewPtr View(const std::string_view fn0, const std::string_view fn1, const std::string_view fn2) { return nullptr; }
    */
};
