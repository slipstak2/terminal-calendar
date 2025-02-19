#include "pch.h"

#include "data-set.h"

class TestSort : public ::testing::Test {
public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(
            FieldDesc::Int("id"),
            FieldDesc::String("name")
        );
        storage->AddRow<int, std::string_view>(1, "Dan4ick");
        storage->AddRow<int, std::string_view>(2, "Igor");
        storage->AddRow<int, std::string_view>(3, "Masha");
        storage->AddRow<int, std::string_view>(4, "Vera");
        storage->AddRow<int, std::string_view>(5, "Yura");
        storage->AddRow<int, std::string_view>(6, "Mitrof");
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestSort::storage;


TEST_F(TestSort, ViewSortBySecondLetter) {
    auto sort_storage = storage->Sort([](const DataFieldAccessor& lhs, const DataFieldAccessor& rhs) {
        return lhs.GetField<std::string_view>("name")[1] < rhs.GetField<std::string_view>("name")[1];
    });

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
        DataRow::Create<int, std::string_view>(3, "Masha"),
        DataRow::Create<int, std::string_view>(4, "Vera"),
        DataRow::Create<int, std::string_view>(2, "Igor"),
        DataRow::Create<int, std::string_view>(6, "Mitrof"),
        DataRow::Create<int, std::string_view>(5, "Yura")
    };

    for (size_t row_num = 0; row_num < sort_storage->RowsCount(); ++row_num) {
        EXPECT_EQ(sort_storage->GetRow(row_num)->GetRow().ToString(), expected[row_num].ToString());
    }
}