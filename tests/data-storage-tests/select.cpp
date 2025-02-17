#include "pch.h"

#include "data-set.h"

class TestSelect : public ::testing::Test {
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
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestSelect::storage;


TEST_F(TestSelect, ViewSelectEvenId) {
    DataContainerPtr view_with_even_id = storage->View()->Select([](const DataFieldAccessor& row) {
        return row.GetField<int>("id") % 2 == 0;
    });

    EXPECT_EQ(2, view_with_even_id->FieldsCount());
    EXPECT_EQ(2, view_with_even_id->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(2, "Igor"),
        DataRow::Create<int, std::string_view>(4, "Vera")
    };
    for (size_t row_num = 0; row_num < view_with_even_id->RowsCount(); ++row_num) {
        EXPECT_EQ(view_with_even_id->GetRow(row_num)->GetRow(), expected[row_num]);
    }
}
