#pragma once
#include "data-field.h"
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <deque>


class StringHeapStorage {
public:
    std::string_view Add(std::string_view sv) {
        data.emplace_back(sv);
        return data.back();
    }
protected:
    std::deque<std::string> data;
};

class DataRow {
public:
    DataRow(std::initializer_list<FieldDesc> l) {
        fields.reserve(l.size());
        for (const FieldDesc& fd : l) {
            fields.push_back({ {.type = fd.type} });
        }
    }

    template<typename T>
    const T& GetField(size_t num);

    template<>
    const int& GetField<int>(size_t num) {
        return fields[num].val.Int;
    }

    template<>
    const std::string_view& GetField<std::string_view>(size_t num) {
        return fields[num].val.String;
    }

    template<>
    const double& GetField<double>(size_t num) {
        return fields[num].val.Double;
    }

    template<>
    const storage::date& GetField<storage::date>(size_t num) {
        return fields[num].val.Date;
    }

    template<typename T>
    void SetField(size_t num, T value);

    template<>
    void SetField<int>(size_t num, int value) {
        CheckType<int>(fields[num].header.type);
        fields[num].val.Int = value;
    }

    template<>
    void SetField<std::string>(size_t num, std::string value) {
        CheckType<std::string_view>(fields[num].header.type);
        fields[num].val.String = stringStorage.Add(std::move(value));
    }

    template<>
    void SetField<std::string_view>(size_t num, std::string_view value) {
        CheckType<std::string_view>(fields[num].header.type);
        fields[num].val.String = stringStorage.Add(value);
    }

    template<>
    void SetField<double>(size_t num, double value) {
        CheckType<double>(fields[num].header.type);
        fields[num].val.Double= value;
    }

    template<>
    void SetField<storage::date>(size_t num, storage::date value) {
        CheckType<storage::date>(fields[num].header.type);
        fields[num].val.Date = value;
    }

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
    void Fill(Types... args) {
        FillImpl(0, args...);
    }

    size_t FieldsCount() {
        return fields.size();
    }

    bool operator == (const DataRow& other) const {
        return fields == other.fields;
    }

protected:
    std::vector<DataField> fields;
    static StringHeapStorage stringStorage;
};

class DataStorage {
public:
    using Ptr = std::shared_ptr<DataStorage>;

public:

    DataStorage(std::initializer_list<FieldDesc> l) : row_dummy(l) {
        ds_fields_desc.reserve(l.size());

        for (const FieldDesc& fd : l) {
            if (ds_fields_mapping.contains(fd.name)) {
                std::string message = "Field with name " + std::string(fd.name) + " already exists";
                throw std::runtime_error(message);
            }

            ds_fields_desc.push_back(fd);
            ds_fields_mapping[ds_fields_desc.back().name] = ds_fields_desc.size() - 1;
        }
    }
    DataRow& CreateEmptyRow() {
        rows.emplace_back(row_dummy);
        return rows.back();
    }

    template<typename... Types>
    DataRow& CreateRow(Types... args) {
        DataRow& row = CreateEmptyRow();
        row.Fill(args...);
        return row;
    }

    template<typename T>
    const T& GetField(DataRow& row, std::string_view field_name) {
        return row.GetField<T>(ds_fields_mapping[field_name]);
    }

    const DataRow& GetRow(size_t idx) const {
        return rows[idx];
    }

    size_t RowsCount() {
        return rows.size();
    }

    size_t FieldsCount() {
        return ds_fields_desc.size();
    }

    const FieldDesc& Field(size_t idx) {
        return ds_fields_desc[idx];
    }
    
protected:
    DataRow row_dummy;
    std::vector<DataRow> rows;

    std::vector<FieldDesc> ds_fields_desc;
    std::map<std::string_view, size_t> ds_fields_mapping;
};