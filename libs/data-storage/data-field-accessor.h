#pragma once
#include <string_view>

class DataRow;

class DataFieldAccessorBase {
public:
    virtual size_t FieldsCount() const = 0;
    virtual DataRow GenRow() = 0;

    FieldType GetFieldType(int field_num) const {
        return GetFieldData(field_num).type;
    }

    template<typename T>
    T GetField(int field_num) const {
        FieldData data = GetFieldData(field_num);
        return data.Get<T>();
    }
protected:
    virtual FieldData GetFieldData(int field_num) const = 0;
};

class DataFieldAccessor : public DataFieldAccessorBase { // aka DataRowWrapper
public:
    using DataFieldAccessorBase::GetFieldType;
    using DataFieldAccessorBase::GetField;

    virtual std::string_view GetFieldName(size_t field_num) const = 0;
    
    template<typename T>
    T GetField(const char* field_name) const {
        FieldData data = GetFieldData(field_name);
        return data.Get<T>();
    }

    FieldType GetFieldType(const char* field_name) const {
        return GetFieldData(field_name).type;
    }

protected:
    virtual FieldData GetFieldData(const char* field_name) const = 0;
};