#pragma once
#include <string_view>

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

    virtual size_t FieldsCount() const = 0;
    virtual DataRow GetRow() = 0;

protected:
    virtual const FieldData& GetFieldData(size_t field_num) const = 0;
};


class DataSetRow;

class DataFieldAccessor : public DataFieldAccessorBase {
    friend class DataSetRow;
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