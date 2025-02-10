#include "pch.h"

#include "data-storage.h"
#include "data-view.h"

class TestDataViewRow : public ::testing::Test {
public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(
            FieldDesc::Int("id"),
            FieldDesc::String("name"),
            FieldDesc::Date("birthday")
        );
        storage->AddRow<int, std::string_view, storage::date>(1, "Dan4ick", storage::date(1996, 12, 25));
        storage->AddRow<int, std::string_view, storage::date>(2, "Igor", storage::date(1986, 9, 9));
        storage->AddRow<int, std::string_view, storage::date>(3, "Masha", storage::date(1986, 12, 2));

        view = storage->View(1, 0);
    }
public:
    static DataStoragePtr storage;
    static DataViewPtr view;
};

DataStoragePtr TestDataViewRow::storage;
DataViewPtr TestDataViewRow::view;

TEST_F(TestDataViewRow, CheckSecondRow) {
    DataViewRow row(view, 1);
    EXPECT_EQ(2, row.FieldsCount());

    EXPECT_EQ("name", row.GetFieldName(0));
    EXPECT_EQ("id", row.GetFieldName(1));

    EXPECT_EQ(FieldType::STRING, row.GetFieldType(0));
    EXPECT_EQ(FieldType::STRING, row.GetFieldType("name"));
    EXPECT_EQ(FieldType::INT, row.GetFieldType(1));
    EXPECT_EQ(FieldType::INT, row.GetFieldType("id"));

    EXPECT_EQ("Igor", row.GetField<std::string_view>(0));
    EXPECT_EQ("Igor", row.GetField<std::string_view>("name"));
    EXPECT_EQ(2, row.GetField<int>(1));
    EXPECT_EQ(2, row.GetField<int>("id"));

    DataRow full_row = row.GetRow();
    DataRow expect_row = DataRow::Create<std::string_view, int>("Igor", 2);

    EXPECT_EQ(expect_row, full_row);
}

/*
TEST_F(TestDataStorageRow, CheckFirstRow) {

    DataRow full_row = row.GetRow();
    DataRow expect_row = DataRow::Create<int, std::string_view, storage::date>(
        1, "Dan4ick", storage::date(1996, 12, 25)
    );

    EXPECT_EQ(expect_row, full_row);
}
*/