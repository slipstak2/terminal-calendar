#pragma once
#include "data-field.h"
#include <vector>
#include <map>
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

struct DataRow {
    std::vector<DataField> fields;
};

class DataStorage {
protected:
    std::vector<DataRow> rows;
    std::vector<std::string> ds_fields_name;
    std::map<std::string_view, int> ds_fields_mapping;

};