#include "pch.h"

#include "data-storage.h"

TEST(TestDataStorage, CreateStringDate) {

    auto storage = DataStorage::Create(
        CreateFieldDesc::String("name"), 
        CreateFieldDesc::Date("birthday")
    );

    EXPECT_EQ(storage->FieldsCount(), 2);

    auto& fd0 = storage->Field(0);
    EXPECT_EQ(fd0.name, "name");
    EXPECT_EQ(fd0.type, FieldType::STRING);

    auto& fd1 = storage->Field(1);
    EXPECT_EQ(fd1.name, "birthday");
    EXPECT_EQ(fd1.type, FieldType::DATE);

    {
        storage::date birhtday = storage::date(1996, 12, 25);
        DataRow& row = storage->AddEmptyRow();
        row.SetField<std::string>(0, "Dan4ick_");
        row.SetField<std::string_view>(0, "Dan4ick"); // override
        row.SetField<storage::date>(1, birhtday);

        std::string_view actual_name = row.GetField<std::string_view>(0);
        EXPECT_EQ(actual_name, "Dan4ick");

        storage::date actual_birthday = row.GetField<storage::date>(1);
        EXPECT_EQ(actual_birthday, birhtday);
    }
    {
        DataRow& row = storage->AddEmptyRow();
        row.SetField<std::string>(0, "Igor");
        row.SetField<storage::date>(1, storage::date(1986, 9, 9));
    }
    {
        DataRow& row = storage->AddEmptyRow();
        row.SetField<std::string>(0, "Masha");
        row.SetField<storage::date>(1, storage::date(1986, 12, 2));
    }
    
    EXPECT_EQ(storage->RowsCount(), 3);
}

TEST(TestDataStorage, CreateIntDouble) {

    auto storage = DataStorage::Create(CreateFieldDesc::Int("id"), CreateFieldDesc::Double("height"));

    EXPECT_EQ(storage->FieldsCount(), 2);

    auto& fd0 = storage->Field(0);
    EXPECT_EQ(fd0.name, "id");
    EXPECT_EQ(fd0.type, FieldType::INT);

    auto& fd1 = storage->Field(1);
    EXPECT_EQ(fd1.name, "height");
    EXPECT_EQ(fd1.type, FieldType::DOUBLE);

    {
        DataRow& row = storage->AddEmptyRow();
        row.SetField<int>(0, 42);
        row.SetField<double>(1, 175.5);

        int actual_id = row.GetField<int>(0);
        EXPECT_EQ(actual_id, 42);

        double actual_height = row.GetField<double>(1);
        EXPECT_EQ(actual_height, 175.5);
    }
    
    EXPECT_EQ(storage->RowsCount(), 1);
}

TEST(TestDataStorage, CreateRow) {

    auto storage = DataStorage::Create(CreateFieldDesc::String("name"), CreateFieldDesc::Int("age"));
    {
        DataRow& row = storage->AddRow<std::string, int>("Dan4ick", 28);
        EXPECT_EQ(row.FieldsCount(), 2);
        const DataRow& row_actual = storage->GetDataRow(0);
        DataFieldAccessorPtr row_access = storage->GetRow(0);
        EXPECT_EQ(row, row_actual);
        
        std::string_view name_by_index = row.GetField<std::string_view>(0);
        std::string_view name_by_field_name = row_access->GetField<std::string_view>("name");

        EXPECT_EQ(name_by_index, "Dan4ick");
        EXPECT_EQ(name_by_index, name_by_field_name);
        
        int age_by_index = row.GetField<int>(1);
        int age_by_field_name = row_access->GetField<int>("age");
        EXPECT_EQ(age_by_index, 28);
        EXPECT_EQ(age_by_index, age_by_field_name);
    }

    EXPECT_EQ(storage->RowsCount(), 1);
}


/*
    DataStorage<std::string, int> storage;
    storage.Add(std::make_tuple<std::string, date, double>("Dan4ick",    {25.12.1996},   178.5));
    storage.Add(std::make_tuple<std::string, date, double>("Igor",       {09.09.1986},   185.2));
    storage.Add(std::make_tuple<std::string, date, double>("Masha",      {02.12.1986},   175.0));

    DataView<2, 0, 1> view(storage);

    view == {
        tuple<double, std::string, date>(178.5,     "Dan4ick",    {25.12.1996}),
        tuple<double, std::string, date>(185.2,     "Igor",       {09.09.1986}),
        tuple<double, std::string, date>(175.0,     "Masha",      {02.12.1986})
    }

    auto genAge = [](const view::RowT& row) -> int {
        return time::now() - row.getBirthday();
    }
    DataSet<short> dataSet(view, genAge);

    view == {
        tuple<double, std::string, date, int>(178.5,     "Dan4ick",    {25.12.1996},    28),
        tuple<double, std::string, date, int>(185.2,     "Igor",       {09.09.1986},    38),
        tuple<double, std::string, date, int>(175.0,     "Masha",      {02.12.1986},    38)

    }

    auto byName = [](const auto& row1, const auto& row2) {
        return row1.getName() < row2.getName();
    }

    view2 = storage.View<2, 0, 1>().Set(genAge).Sort(byName).View<1, 3>()

    view2 == {
        tuple<std::string, int>("Dan4ick",  28),
        tuple<std::string, int>("Igor",     38),
        tuple<std::string, int>("Masha,     38)
    }

    auto singleAge = [](const auto& row) {
        return row.getAge();
    }
    view3 = view2.Unique(singleAge)

    view3 == {
        tuple<std::string, int>("Dan4ick",  28),
        tuple<std::string, int>("Igor",     38),
    }

    auto selectFn = [](const auto& row){
        row.getName() == "Dan4ick"
    }
    view4 = view3.Select(selectFn).View<1>();

    view4 == {
        tuple<int>(28),
    }

*/