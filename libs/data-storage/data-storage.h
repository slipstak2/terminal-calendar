#pragma once
#include "data-field.h"
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <deque>
/*

struct DataRow {
    std::vector<Field> fields;
}

struct DataRowEx {
    DataRow data_row;
    std::map<std::string_view, *Field> row_fields_mapping;
}

class DataStorage {
    std::vector<DataRow> rows;

    std::vector<std::string> ds_fields_name;
    std::map<std::string_view, int> ds_fields_mapping;
}


DataStorage storage(
    CreateField::String("name"),
    CreateField::Date("birthday"),
    CreateField::Double("height")
);

storage.Add(MakeRow<std::string, date, double>("Dan4ick",    {25.12.1996},   178.5));
storage.Add(MakeRow<std::string, date, double>("Igor",       {09.09.1986},   185.2));
storage.Add(MakeRow<std::string, date, double>("Masha",      {02.12.1986},   175.0));

*/

class StringHeapStorage {

public:
    std::string_view Add(std::string s) {
        data.emplace_back(s);
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
    T& GetField(size_t num);

    template<>
    int& GetField<int>(size_t num) {
        return fields[num].val.Int;
    }

    template<>
    std::string_view& GetField<std::string_view>(size_t num) {
        return fields[num].val.String;
    }

    template<>
    double& GetField<double>(size_t num) {
        return fields[num].val.Double;
    }

    template<>
    storage::date& GetField<storage::date>(size_t num) {
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
    void SetField<double>(size_t num, double value) {
        CheckType<double>(fields[num].header.type);
        fields[num].val.Double= value;
    }

    template<>
    void SetField<storage::date>(size_t num, storage::date value) {
        CheckType<storage::date>(fields[num].header.type);
        fields[num].val.Date = value;
    }

    template<size_t...I/*, typename... Types*/>
    void Fill(std::index_sequence<I...>/*, Types... args*/) {

    }

protected:
    std::vector<DataField> fields;
    static StringHeapStorage stringStorage;
};

class DataStorage {
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
        //row.Fill<Types...>(std::make_index_sequence<sizeof...(Types)>, args...);
        row.Fill(std::make_index_sequence<sizeof...(Types)>());
        return row;
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
    std::map<std::string_view, int> ds_fields_mapping;

};