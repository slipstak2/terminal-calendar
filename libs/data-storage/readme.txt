
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


