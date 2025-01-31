#pragma once

#include "common.h"
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


class DataRowBuilder;

class DataRow {
    friend class DataRowBuilder;

public:
    DataRow() = default;
    DataRow(std::initializer_list<FieldDesc> l) {
        fields.reserve(l.size());
        for (const FieldDesc& fd : l) {
            fields.push_back({ {.type = fd.type} });
        }
    }

    template<typename T>
    const T& GetField(size_t num) const;

    template<>
    const int& GetField<int>(size_t field_idx) const{
        return fields[field_idx].val.Int;
    }

    template<>
    const std::string_view& GetField<std::string_view>(size_t field_idx) const{
        return fields[field_idx].val.String;
    }

    template<>
    const double& GetField<double>(size_t field_idx) const{
        return fields[field_idx].val.Double;
    }

    template<>
    const storage::date& GetField<storage::date>(size_t field_idx) const{
        return fields[field_idx].val.Date;
    }

    const DataField& GetRawField(size_t field_idx) const {
        return fields[field_idx];
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

    void ReserveFieldsCount(size_t fields_count) {
        fields.reserve(fields_count);
    }

    bool operator == (const DataRow& other) const {
        return fields == other.fields;
    }

private:
    void AddField(const DataField& field) {
        fields.emplace_back(field);
    }

protected:
    std::vector<DataField> fields;
    static StringHeapStorage stringStorage;
};

//
class DataRowBuilder {
public:
    DataRowBuilder() = default;
    DataRowBuilder(size_t fields_count) {
        result.ReserveFieldsCount(fields_count);
    }
    void Add(const DataField& field) {
        result.AddField(field);
    }
    const DataRow& Result() const {
        return result;
    }
protected:
    DataRow result;
};

class DataStorage: public std::enable_shared_from_this<DataStorage> {
public:
    static std::shared_ptr<DataStorage> Create(std::initializer_list<FieldDesc> fds) {
        return std::shared_ptr<DataStorage>(new DataStorage(fds));
    }

public:
    DataViewPtr View(std::vector<size_t>&& fields_idx);

    DataRow& AddEmptyRow() {
        rows.emplace_back(row_dummy);
        return rows.back();
    }

    template<typename... Types>
    DataRow& AddRow(Types... args) {
        DataRow& row = AddEmptyRow();
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

    DataRow GetRow(size_t idx, const std::vector<size_t>& fields_idx) {
        const DataRow& row = rows[idx];
        DataRowBuilder builder(fields_idx.size());
        for (size_t field_idx : fields_idx) {
            builder.Add(row.GetRawField(field_idx));
        }
        return builder.Result();
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

private:
    explicit DataStorage(std::initializer_list<FieldDesc> fds) : row_dummy(fds) {
        ds_fields_desc.reserve(fds.size());

        for (const FieldDesc& fd : fds) {
            if (ds_fields_mapping.contains(fd.name)) {
                std::string message = "Field with name " + std::string(fd.name) + " already exists";
                throw std::runtime_error(message);
            }

            ds_fields_desc.push_back(fd);
            ds_fields_mapping[ds_fields_desc.back().name] = ds_fields_desc.size() - 1;
        }
    }
    
protected:
    DataRow row_dummy;
    std::vector<DataRow> rows;

    std::vector<FieldDesc> ds_fields_desc;
    std::map<std::string_view, size_t> ds_fields_mapping;
};