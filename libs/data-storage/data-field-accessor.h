#pragma once
#include <string_view>
#include "data-row.h"

class DataFieldAccessor { // aka DataRowWrapper
public:
    virtual size_t FieldsCount() = 0;

    template<typename T>
    T GetField(int field_num) {
        FieldData data = GetFieldData(field_num);
        return data.Get<T>();
    }

    template<typename T>
    T GetField(const char* field_name) {
        FieldData data = GetFieldData(field_name);
        return data.Get<T>();
    }

    FieldType GetFieldType(int field_num) {
        return GetFieldData(field_num).type;
    }

    FieldType GetFieldType(const char* field_name) {
        return GetFieldData(field_name).type;
    }

    virtual std::string_view GetFieldName(size_t field_num) = 0;
    virtual DataRow GenRow() = 0;

protected:
    virtual FieldData GetFieldData(int field_num) = 0;
    virtual FieldData GetFieldData(const char* field_name) = 0;

};