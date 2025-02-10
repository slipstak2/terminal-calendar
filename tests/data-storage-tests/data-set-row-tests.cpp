#include "pch.h"

#include "data-set.h"

class TestDataSetRow : public ::testing::Test {
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

        dataSet = DataSet::Create(storage->View(2, 1));
    }
public:
    static DataStoragePtr storage;
    static DataSetPtr dataSet;
};

DataStoragePtr TestDataSetRow::storage;
DataSetPtr TestDataSetRow::dataSet;

TEST_F(TestDataSetRow, CheckThirdRow) {

    DataSetRow row(dataSet, 2);
    EXPECT_EQ(2, row.FieldsCount());

    EXPECT_EQ("birthday", row.GetFieldName(0));
    EXPECT_EQ("name", row.GetFieldName(1));

    EXPECT_EQ(FieldType::DATE, row.GetFieldType(0));
    EXPECT_EQ(FieldType::DATE, row.GetFieldType("birthday"));
    EXPECT_EQ(FieldType::STRING, row.GetFieldType(1));
    EXPECT_EQ(FieldType::STRING, row.GetFieldType("name"));

    EXPECT_EQ(storage::date(1986, 12, 2), row.GetField<storage::date>(0));
    EXPECT_EQ(storage::date(1986, 12, 2), row.GetField<storage::date>("birthday"));
    EXPECT_EQ("Masha", row.GetField<std::string_view>(1));
    EXPECT_EQ("Masha", row.GetField<std::string_view>("name"));

    DataRow full_row = row.GetRow();
    DataRow expect_row = DataRow::Create<storage::date, std::string_view>(storage::date(1986, 12, 2), "Masha");

    EXPECT_EQ(expect_row, full_row);
}

TEST_F(TestDataSetRow, AddColumn) {
    auto dataSet = DataSet::Create(storage->View());
    EXPECT_EQ(3, dataSet->RowsCount());
    EXPECT_EQ(3, dataSet->FieldsCount());

    dataSet->AddColumn(FieldDesc::Int("birthday_day"), [](const DataFieldAccessor& row) {
        return row.GetField<storage::date>("birthday").day();
        });

    EXPECT_EQ(3, dataSet->RowsCount());
    EXPECT_EQ(4, dataSet->FieldsCount());

    DataFieldAccessorPtr row = dataSet->GetRow(0);
    EXPECT_EQ(4, row->FieldsCount());

    EXPECT_EQ("id", row->GetFieldName(0));
    EXPECT_EQ("name", row->GetFieldName(1));
    EXPECT_EQ("birthday", row->GetFieldName(2));
    EXPECT_EQ("birthday_day", row->GetFieldName(3));

    EXPECT_EQ(FieldType::INT, row->GetFieldType(0));
    EXPECT_EQ(FieldType::INT, row->GetFieldType("id"));
    EXPECT_EQ(FieldType::STRING, row->GetFieldType(1));
    EXPECT_EQ(FieldType::STRING, row->GetFieldType("name"));
    EXPECT_EQ(FieldType::DATE, row->GetFieldType(2));
    EXPECT_EQ(FieldType::DATE, row->GetFieldType("birthday"));
    EXPECT_EQ(FieldType::INT, row->GetFieldType(3));
    EXPECT_EQ(FieldType::INT, row->GetFieldType("birthday_day"));

    EXPECT_EQ(1, row->GetField<int>(0));
    EXPECT_EQ(1, row->GetField<int>("id"));
    EXPECT_EQ("Dan4ick", row->GetField<std::string_view>(1));
    EXPECT_EQ("Dan4ick", row->GetField<std::string_view>("name"));
    EXPECT_EQ(storage::date(1996, 12, 25), row->GetField<storage::date>(2));
    EXPECT_EQ(storage::date(1996, 12, 25), row->GetField<storage::date>("birthday"));
    EXPECT_EQ(25, row->GetField<int>(3));
    EXPECT_EQ(25, row->GetField<int>("birthday_day"));

    DataRow full_row = row->GetRow();
    DataRow expect_row = DataRow::Create<int, std::string_view, storage::date, int>(
        1, "Dan4ick", storage::date(1996, 12, 25), 25);

    EXPECT_EQ(expect_row, full_row);
}