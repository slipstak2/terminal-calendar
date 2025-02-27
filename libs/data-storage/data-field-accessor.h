#pragma once

#include "data-field.h"

#include <string_view>
#include <vector>

class DataRow;

class DataFieldAccessorBase {
public:
    template<typename T>
    const T& GetField(size_t field_num) const {
        return GetFieldData(field_num).Get<T>();
    }

    FieldType GetFieldType(size_t field_num) const {
        return GetFieldData(field_num).type;
    }

    std::string ToString() const;

    virtual size_t FieldsCount() const = 0;

    virtual DataRow GetRow() const = 0;
    DataRow GetRow(const std::vector<size_t>& fields_num) const;

protected:
    virtual const FieldData& GetFieldData(size_t field_num) const = 0;
};


class DataSetRow;
class DataViewRow;

class DataFieldAccessor : public DataFieldAccessorBase {
    friend class DataSetRow;
    friend class DataViewRow;
public:
    using DataFieldAccessorBase::GetField;
    using DataFieldAccessorBase::GetFieldType;

    template<typename T>
    const T& GetField(const std::string_view field_name) const {
        return GetFieldData(GetFieldIndex(field_name)).Get<T>();
    }

    FieldType GetFieldType(const std::string_view field_name) const {
        return GetFieldData(GetFieldIndex(field_name)).type;
    }

    virtual std::string_view GetFieldName(size_t field_num) const = 0;

protected:
    virtual size_t GetFieldIndex(const std::string_view field_name) const = 0;
};