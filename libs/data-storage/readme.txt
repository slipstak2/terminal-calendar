
enum class FieldType {
    INT,
    DOUBLE
    DATE
};

struct Header {
    FieldType t;
}

union FieldValue {
    int Int;
    double Double;
    date Date;
}

struct Field {
    Header h;
    FieldValue val;
    
}

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


class DataView {
    const DataStorage& ds;
    std::vector<size_t> rowsIds;

    std::vector<std::string_view> dv_fields_name;
    std::map<std::string_view, int> dv_fields_mapping;
}

DataView view(storage, {"height", "name", "birthday"});
DataView view(storage, {2, 0, 1});

class DataSet {
    const DataView& dv;
    DataStorage localStorage;
}

view.Add(CreateField::Int("age", [view.FieldAccessor()](const DataRow& row){
    Now() - fieldAccessor(row, "birthday");
}))



========  Select  =======
.Select(fieldsName/fieldsNum);

storage.Select()    -> data_set(view)
view.Select()       -> data_set(view')
data_set.Select()   -> data_set(view')

========  Add  =======

storage.Select().Add().Add();

view.Add()      -> dataSet()
dataSet.Add()   -> dataSet();

========  Unique  ======
.Unique()                   // by firstField
.Unique(fieldName/fieldNum)
.Unique(fieldsName/fieldsNum) .Unique({"name", "age"})

storage.Unique()    ->  view
view.Unique()       ->  view'
data_set.Unique()

========  Sort  ======
.Sort(fieldName/fieldNum + Order)       .Sort({"name", ASC}, {age, DESC});
.Sort(compareFn)                        .Sort([](const DataRow& row1, const DataRow& row2));


