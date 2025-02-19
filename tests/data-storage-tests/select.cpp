#include "pch.h"

#include "utils.h"
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
        storage->AddRow<int, std::string_view>(6, "Mitrof");
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
    EXPECT_EQ(3, view_with_even_id->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(2, "Igor"),
        DataRow::Create<int, std::string_view>(4, "Vera"),
        DataRow::Create<int, std::string_view>(6, "Mitrof")
    };

    CHECK_EQ(expected, view_with_even_id);
}

TEST_F(TestSelect, ViewSelect2) {
    auto select_even_id = [](const DataFieldAccessor& row) {
        return row.GetField<int>("id") % 2 == 0;
    };
    auto select_mod3_id = [](const DataFieldAccessor& row) {
        return row.GetField<int>("id") % 3 == 0;
    };
    DataContainerPtr view_with_select2 = storage->View()->Select(select_even_id)->Select(select_mod3_id);

    EXPECT_EQ(2, view_with_select2->FieldsCount());
    EXPECT_EQ(1, view_with_select2->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(6, "Mitrof")
    };
    CHECK_EQ(expected, view_with_select2);
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
    CHECK_EQ(expected, storage_with_odd_id);
}

TEST_F(TestSelect, StorageSelect2) {
    auto select_odd_id = [](const DataFieldAccessor& row) {
        return row.GetField<int>("id") % 2 == 1;
    };
    auto select_mod3_id = [](const DataFieldAccessor& row) {
        return row.GetField<int>("id") % 3 == 0;
    };
    DataContainerPtr storage_with_select2 = storage->Select(select_odd_id)->Select(select_mod3_id);

    EXPECT_EQ(2, storage_with_select2->FieldsCount());
    EXPECT_EQ(1, storage_with_select2->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(3, "Masha"),
    };
    CHECK_EQ(expected, storage_with_select2);
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
    CHECK_EQ(expected, dataset_with_last_a);
}

TEST_F(TestSelect, SetSelec2) {
    auto select_last_a = [](const DataFieldAccessor& row) {
        return row.GetField<std::string_view>("name").back() == 'a';
    };
    auto select_first_v = [](const DataFieldAccessor& row) {
        return row.GetField<std::string_view>("name").front() == 'V';
    };
    DataContainerPtr dataset_with_select2 = DataSet::Create(storage->View())->Select(select_last_a)->Select(select_first_v);

    EXPECT_EQ(2, dataset_with_select2->FieldsCount());
    EXPECT_EQ(1, dataset_with_select2->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(4, "Vera"),
    };
    CHECK_EQ(expected, dataset_with_select2);
}
