#include "pch.h"

#include "data-set.h"
#include "utils.h"

class TestUnique : public ::testing::Test {
public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(
            FieldDesc::Int("id"),
            FieldDesc::String("name"),
            FieldDesc::Int("gender")
        );
        storage->AddRow<int, std::string_view, int>(1, "Dan4ick", 1);
        storage->AddRow<int, std::string_view, int>(2, "Igor", 1);
        storage->AddRow<int, std::string_view, int>(3, "Masha", 0);
        storage->AddRow<int, std::string_view, int>(4, "Vera", 0);
        storage->AddRow<int, std::string_view, int>(5, "Yura", 1);
        storage->AddRow<int, std::string_view, int>(4, "Vera-duplicate", 0);
        storage->AddRow<int, std::string_view, int>(6, "Mitrof", 1);
        storage->AddRow<int, std::string_view, int>(2, "Igor-copy", 1);
        storage->AddRow<int, std::string_view, int>(7, "Yura", 1);
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestUnique::storage;

TEST_F(TestUnique, StorageUniqueGender) {
    auto unique_gender_storage = Unique<int>(storage, "gender");
    
    EXPECT_EQ(3, unique_gender_storage->FieldsCount());
    EXPECT_EQ(2, unique_gender_storage->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view, int>(1, "Dan4ick", 1),
        DataRow::Create<int, std::string_view, int>(3, "Masha", 0)
    };

    CHECK_EQ(expected, unique_gender_storage);

    auto unique_field2_storage = Unique<int>(storage, 2);
    CHECK_EQ(unique_gender_storage, unique_field2_storage);
}

TEST_F(TestUnique, ViewUniqueId) {
    auto unique_id_view = Unique<int>(storage->View(), "id");

    EXPECT_EQ(3, unique_id_view->FieldsCount());
    EXPECT_EQ(7, unique_id_view->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view, int>(1, "Dan4ick", 1),
        DataRow::Create<int, std::string_view, int>(2, "Igor", 1),
        DataRow::Create<int, std::string_view, int>(3, "Masha", 0),
        DataRow::Create<int, std::string_view, int>(4, "Vera", 0),
        DataRow::Create<int, std::string_view, int>(5, "Yura", 1),
        DataRow::Create<int, std::string_view, int>(6, "Mitrof", 1),
        DataRow::Create<int, std::string_view, int>(7, "Yura", 1)
    };

    CHECK_EQ(expected, unique_id_view);

    auto unique_field0_view = Unique<int>(storage->View(), 0);
    CHECK_EQ(unique_id_view, unique_field0_view);
    auto unique_view = Unique<int>(storage->View());
    CHECK_EQ(unique_id_view, unique_view);
}

TEST_F(TestUnique, SetUniqueId) {
    auto unique_name_dataset = Unique<std::string_view>(DataSet::Create(storage->View()), "name");

    EXPECT_EQ(3, unique_name_dataset->FieldsCount());
    EXPECT_EQ(8, unique_name_dataset->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view, int>(1, "Dan4ick", 1),
        DataRow::Create<int, std::string_view, int>(2, "Igor", 1),
        DataRow::Create<int, std::string_view, int>(3, "Masha", 0),
        DataRow::Create<int, std::string_view, int>(4, "Vera", 0),
        DataRow::Create<int, std::string_view, int>(5, "Yura", 1),
        DataRow::Create<int, std::string_view, int>(4, "Vera-duplicate", 0),
        DataRow::Create<int, std::string_view, int>(6, "Mitrof", 1),
        DataRow::Create<int, std::string_view, int>(2, "Igor-copy", 1)
    };

    CHECK_EQ(expected, unique_name_dataset);

    auto unique_field1_dataset = Unique<std::string_view>(DataSet::Create(storage->View()), 1);
    CHECK_EQ(unique_name_dataset, unique_field1_dataset);
}
