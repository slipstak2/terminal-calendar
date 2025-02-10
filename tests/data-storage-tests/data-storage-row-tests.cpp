#include "pch.h"

#include "data-storage.h"

class TestDataStorageRow : public ::testing::Test {
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
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestDataStorageRow::storage;

TEST_F(TestDataStorageRow, CheckFirstRow) {
    DataStorageRow row(storage, 0);
    EXPECT_EQ(3, row.FieldsCount());

    EXPECT_EQ("id", row.GetFieldName(0));
    EXPECT_EQ("name", row.GetFieldName(1));
    EXPECT_EQ("birthday", row.GetFieldName(2));

    EXPECT_EQ(FieldType::INT, row.GetFieldType(0));
    EXPECT_EQ(FieldType::INT, row.GetFieldType("id"));
    EXPECT_EQ(FieldType::STRING, row.GetFieldType(1));
    EXPECT_EQ(FieldType::STRING, row.GetFieldType("name"));
    EXPECT_EQ(FieldType::DATE, row.GetFieldType(2));
    EXPECT_EQ(FieldType::DATE, row.GetFieldType("birthday"));

    EXPECT_EQ(1, row.GetField<int>(0));
    EXPECT_EQ(1, row.GetField<int>("id"));
    EXPECT_EQ("Dan4ick", row.GetField<std::string_view>(1));
    EXPECT_EQ("Dan4ick", row.GetField<std::string_view>("name"));
    EXPECT_EQ(storage::date(1996, 12, 25), row.GetField<storage::date>(2));
    EXPECT_EQ(storage::date(1996, 12, 25), row.GetField<storage::date>("birthday"));

    DataRow full_row = row.FullRow();
    DataRow expect_row = DataRow::Create<int, std::string_view, storage::date>(
        1, "Dan4ick", storage::date(1996, 12, 25)
    );

    EXPECT_EQ(expect_row, full_row);
}