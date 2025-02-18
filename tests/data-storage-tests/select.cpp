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

TEST_F(TestSelect, StorageSelectOddId) {
    DataContainerPtr storage_with_odd_id = storage->Select([](const DataFieldAccessor& row) {
        return row.GetField<int>("id") % 2 == 1;
        });

    EXPECT_EQ(2, storage_with_odd_id->FieldsCount());
    EXPECT_EQ(3, storage_with_odd_id->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
        DataRow::Create<int, std::string_view>(3, "Masha"),
        DataRow::Create<int, std::string_view>(5, "Yura")
    };
    for (size_t row_num = 0; row_num < storage_with_odd_id->RowsCount(); ++row_num) {
        EXPECT_EQ(storage_with_odd_id->GetRow(row_num)->GetRow(), expected[row_num]);
    }
}

TEST_F(TestSelect, SetSelectLastA) {
    DataContainerPtr dataset_with_last_a = DataSet::Create(storage->View())->Select([](const DataFieldAccessor& row) {
        return row.GetField<std::string_view>("name").back() == 'a';
        });

    EXPECT_EQ(2, dataset_with_last_a->FieldsCount());
    EXPECT_EQ(3, dataset_with_last_a->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(3, "Masha"),
        DataRow::Create<int, std::string_view>(4, "Vera"),
        DataRow::Create<int, std::string_view>(5, "Yura")
    };
    for (size_t row_num = 0; row_num < dataset_with_last_a->RowsCount(); ++row_num) {
        EXPECT_EQ(dataset_with_last_a->GetRow(row_num)->GetRow(), expected[row_num]);
    }
}
