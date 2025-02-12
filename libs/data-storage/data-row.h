#pragma once

#include "common.h"
#include "data-field.h"
#include "data-field-accessor.h"

#include <vector>
#include <string>
#include <string_view>
#include <map>

class DataStorage;

class DataRow : public DataFieldAccessorBase {
    friend class DataStorage;
public:
    using DataFieldAccessorBase::GetRow;

public:
    DataRow() = default;
    DataRow(size_t fields_count) {
        ReserveFieldsCount(fields_count);
    }

    size_t FieldsCount() const override {
        return fields.size();
    }

    DataRow GetRow() const override {
        return *this;
    }

    const FieldData& GetFieldData(size_t field_idx) const override {
        return fields[field_idx];
    }

    template<typename T>
    void SetField(size_t field_num, T value, bool initType = false) {
        if (initType) {
            if (fields.size() == field_num) {
                AddFieldType(FieldTyper(value));
            } else {
                fields[field_num].type = FieldTyper(value); // or assert?
            }
        }
        FieldData& fieldData = fields[field_num];
        if (!initType) {
            CheckType<T>(fieldData.type);
        }
        fieldData.Set<T>(value);
    }

    void ReserveFieldsCount(size_t fields_count) {
        fields.reserve(fields_count);
    }

    void AddFieldType(const FieldType& field_type) {
        fields.emplace_back(field_type);
    }

    void AddFieldData(const FieldData& field_data) {
        fields.emplace_back(field_data);
    }

    bool operator == (const DataRow& other) const {
        return fields == other.fields;
    }

    template<typename... Types>
    void Fill(Types... args) {
        FillImpl(0, args...);
    }

    template<typename... Types>
    static DataRow Create(Types... args) {
        DataRow result(sizeof...(args));
        result.CreateInternal(args...);
        return result;
    }
private:
    template<typename T>
    void FillImpl(size_t num, T t) {
        SetField<T>(num, t);
    }

    template<typename T, typename ...Types>
    void FillImpl(size_t num, T t, Types... args) {
        FillImpl(num, t);
        FillImpl(num + 1, args...);
    }

    template<typename... Types>
    void CreateInternal(Types... args) {
        CreateImpl(0, args...);
    }

    template<typename T>
    void CreateImpl(size_t num, T t) {
        SetField<T>(num, t, true);
    }

    template<typename T, typename ...Types>
    void CreateImpl(size_t num, T t, Types... args) {
        CreateImpl(num, t);
        CreateImpl(num + 1, args...);
    }

protected:
    std::vector<FieldData> fields;
};
