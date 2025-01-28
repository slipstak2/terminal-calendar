#include "pch.h"

#include "data-storage.h"

TEST(TestDataStorage, Create) {

    DataStorage storage({ CreateField::String("name"), CreateField::Date("birthday") });

    EXPECT_EQ(storage.FieldsCount(), 2);

    auto& fd0 = storage.Field(0);
    EXPECT_EQ(fd0.name, "name");
    EXPECT_EQ(fd0.type, FieldType::STRING);

    auto& fd1 = storage.Field(1);
    EXPECT_EQ(fd1.name, "birthday");
    EXPECT_EQ(fd1.type, FieldType::DATE);

    {
        DataRow& row = storage.CreateRow();
        row.SetField<std::string>(0, "Dan4ick");
        row.SetField<std::chrono::year_month_day>(1, std::chrono::year_month_day(
            std::chrono::year(1996),
            std::chrono::month(12),
            std::chrono::day(25)
        ));
    }
    {
        DataRow& row = storage.CreateRow();
        row.SetField<std::string>(0, "Igor");
        row.SetField<std::chrono::year_month_day>(1, std::chrono::year_month_day(
            std::chrono::year(1986),
            std::chrono::month(9),
            std::chrono::day(9)
        ));
    }
    {
        DataRow& row = storage.CreateRow();
        row.SetField<std::string>(0, "Masha");
        row.SetField<std::chrono::year_month_day>(1, std::chrono::year_month_day(
            std::chrono::year(1986),
            std::chrono::month(12),
            std::chrono::day(2)
        ));
    }
    
    EXPECT_EQ(storage.RowsCount(), 3);

    // TODO: check all rows;

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