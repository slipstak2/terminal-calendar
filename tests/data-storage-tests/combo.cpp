#include "pch.h"

#include "data-set.h"
#include "utils.h"

class TestCombo : public ::testing::Test {
public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(
            FieldDesc::Int("id"),
            FieldDesc::String("name"),
            FieldDesc::Date("birthday")
        );
        storage->AddRow<int, std::string_view, storage::date>(1,  "Pushkin",    storage::date(1799, 6, 6));
        storage->AddRow<int, std::string_view, storage::date>(2,  "Lermontov",  storage::date(1814, 15, 10));
        storage->AddRow<int, std::string_view, storage::date>(3,  "Tolstoy",    storage::date(1828, 9, 9));
        storage->AddRow<int, std::string_view, storage::date>(4,  "Dostoevsky", storage::date(1821, 11, 11));
        storage->AddRow<int, std::string_view, storage::date>(5,  "Gogol",      storage::date(1809, 4, 1));
        storage->AddRow<int, std::string_view, storage::date>(6,  "Turgenev",   storage::date(1818, 11, 9));
        storage->AddRow<int, std::string_view, storage::date>(7,  "Chehov",     storage::date(1860, 29, 1));
        storage->AddRow<int, std::string_view, storage::date>(8,  "Kuprin",     storage::date(1870, 9, 7));
        storage->AddRow<int, std::string_view, storage::date>(9,  "Bunin",      storage::date(1870, 10, 22));
        storage->AddRow<int, std::string_view, storage::date>(10, "Zoshchenko", storage::date(1894, 8, 10));
        storage->AddRow<int, std::string_view, storage::date>(11, "Gorkiy",     storage::date(1868, 3, 28));
        storage->AddRow<int, std::string_view, storage::date>(12, "Bulgakov",   storage::date(1891, 5, 15));
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestCombo::storage;

TEST_F(TestCombo, SortAndLimit) {
    auto by_year = [](const DataFieldAccessor& lhs, const DataFieldAccessor& rhs) {
        return lhs.GetField<storage::date>("birthday").year() < rhs.GetField<storage::date>("birthday").year();
    };
    auto sort_and_limit = storage->Sort(by_year)->Limit(2);

    EXPECT_EQ(3, sort_and_limit->FieldsCount());
    EXPECT_EQ(2, sort_and_limit->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view, storage::date>(1, "Pushkin", storage::date(1799, 6, 6)),
        DataRow::Create<int, std::string_view, storage::date>(5, "Gogol", storage::date(1809, 4, 1))
    };

    CHECK_EQ(expected, sort_and_limit);
}

TEST_F(TestCombo, ViewAndSort) {
    auto by_name = [](const DataFieldAccessor& lhs, const DataFieldAccessor& rhs) {
        return lhs.GetField<std::string_view>("name") < rhs.GetField<std::string_view>("name");
    };
    auto view_and_sort_and_limit = storage->View("name")->Sort(by_name)->Limit(2);

    EXPECT_EQ(1, view_and_sort_and_limit->FieldsCount());
    EXPECT_EQ(2, view_and_sort_and_limit->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<std::string_view>("Bulgakov"),
        DataRow::Create<std::string_view>("Bunin")
    };

    CHECK_EQ(expected, view_and_sort_and_limit);
}

TEST_F(TestCombo, AddColumnAndView) {
    int cur_year = 2025;
    auto calc_age = [&cur_year](const DataFieldAccessor& row) {
        storage::date b = row.GetField<storage::date>("birthday");
        return cur_year - b.year();
        };
    auto add_column_and_view = storage->AddColumn(FieldDesc::Int("age"), calc_age)->View("age", "name")->Limit(2);

    EXPECT_EQ(2, add_column_and_view->FieldsCount());
    EXPECT_EQ(2, add_column_and_view->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(226, "Pushkin"),
        DataRow::Create<int, std::string_view>(211, "Lermontov")
    };

    CHECK_EQ(expected, add_column_and_view);
}

TEST_F(TestCombo, AddColumnAndSort) {
    int cur_year = 2025;
    auto calc_age = [&cur_year](const DataFieldAccessor& row) {
        storage::date b = row.GetField<storage::date>("birthday");
        return cur_year - b.year();
    };
    auto by_age = [](const DataFieldAccessor& lhs, const DataFieldAccessor& rhs) {
        return lhs.GetField<int>("age") < rhs.GetField<int>("age");
    };
    auto add_column_and_sort = storage->AddColumn(FieldDesc::Int("age"), calc_age)->Sort(by_age)->Limit(3);

    EXPECT_EQ(4, add_column_and_sort->FieldsCount());
    EXPECT_EQ(3, add_column_and_sort->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view, storage::date, int>(10, "Zoshchenko", storage::date(1894, 8, 10), 131),
        DataRow::Create<int, std::string_view, storage::date, int>(12,"Bulgakov", storage::date(1891, 5, 15), 134),
        DataRow::Create<int, std::string_view, storage::date, int>(8, "Kuprin", storage::date(1870, 9, 7), 155),
    };

    CHECK_EQ(expected, add_column_and_sort);
}

TEST_F(TestCombo, SortAndAddColumn) {
    int cur_year = 2025;
    auto calc_age = [&cur_year](const DataFieldAccessor& row) {
        storage::date b = row.GetField<storage::date>("birthday");
        return cur_year - b.year();
        };
    auto by_birthday = [](const DataFieldAccessor& lhs, const DataFieldAccessor& rhs) {
        return lhs.GetField<storage::date>("birthday") > rhs.GetField<storage::date>("birthday");
        };
    auto sort_and_add_column = storage->Sort(by_birthday)->AddColumn(FieldDesc::Int("age"), calc_age)->Limit(3);

    EXPECT_EQ(4, sort_and_add_column->FieldsCount());
    EXPECT_EQ(3, sort_and_add_column->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view, storage::date, int>(10, "Zoshchenko", storage::date(1894, 8, 10), 131),
        DataRow::Create<int, std::string_view, storage::date, int>(12, "Bulgakov", storage::date(1891, 5, 15), 134),
        DataRow::Create<int, std::string_view, storage::date, int>(9, "Bunin", storage::date(1870, 10, 22), 155),
    };

    CHECK_EQ(expected, sort_and_add_column);
}
TEST_F(TestCombo, ViewAndAddColumnAndUnique) {
    auto calc_first_letter = [](const DataFieldAccessor& row) {
        return std::string(1, row.GetField<std::string_view>("name")[0]);
    };
    auto view_and_add_column_and_unique = storage
        ->View("name")
        ->AddColumn(FieldDesc::String("first_letter"), calc_first_letter)
        ->Unique<std::string_view>("first_letter")
        ->View("first_letter", "name")
    ;

    EXPECT_EQ(2, view_and_add_column_and_unique->FieldsCount());
    EXPECT_EQ(9, view_and_add_column_and_unique->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<std::string_view, std::string_view>("P", "Pushkin"),
        DataRow::Create<std::string_view, std::string_view>("L", "Lermontov"),
        DataRow::Create<std::string_view, std::string_view>("T", "Tolstoy"),
        DataRow::Create<std::string_view, std::string_view>("D", "Dostoevsky"),
        DataRow::Create<std::string_view, std::string_view>("G", "Gogol"),
        DataRow::Create<std::string_view, std::string_view>("C", "Chehov"),
        DataRow::Create<std::string_view, std::string_view>("K", "Kuprin"),
        DataRow::Create<std::string_view, std::string_view>("B", "Bunin"),
        DataRow::Create<std::string_view, std::string_view>("Z", "Zoshchenko"),
    };

    CHECK_EQ(expected, view_and_add_column_and_unique);
}
