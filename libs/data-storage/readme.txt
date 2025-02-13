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


