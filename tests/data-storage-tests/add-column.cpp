#include "pch.h"

#include "data-set.h"

class TestAddColumn : public ::testing::Test {
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

DataStoragePtr TestAddColumn::storage;

TEST_F(TestAddColumn, StorageAddSingleColumn) {
    DataContainerPtr storage_with_column = storage->AddColumn(FieldDesc::Int("id_plus_42"), [](const DataFieldAccessor& row) {
        return row.GetField<int>("id") + 42;
    });
    EXPECT_EQ(4, storage_with_column->FieldsCount());
    DataFieldAccessorPtr row = storage_with_column->GetRow(1);
    EXPECT_EQ(44, row->GetField<int>(3));
    EXPECT_EQ(44, row->GetField<int>("id_plus_42"));
    DataRow expected = DataRow::Create<int, std::string_view, storage::date, int>(2, "Igor", storage::date(1986, 9, 9), 2+42);
    EXPECT_EQ(row->GetRow(), expected);
}

TEST_F(TestAddColumn, StorageAddTwoColumns) {
    auto id_plus_42_cb = [](const DataFieldAccessor& row) {
        return row.GetField<int>("id") + 42;
    };
    auto reverse_name_cb = [](const DataFieldAccessor& row) {
        std::string name(row.GetField<std::string_view>("name"));
        reverse(name.begin(), name.end());
        return name;
    };
    DataContainerPtr storage_with_2column = 
        storage->
        AddColumn(FieldDesc::Int("id_plus_42"), id_plus_42_cb)->
        AddColumn(FieldDesc::String("reverse name"), reverse_name_cb);

    EXPECT_EQ(5, storage_with_2column->FieldsCount());
    DataFieldAccessorPtr row = storage_with_2column->GetRow(1);
    EXPECT_EQ(44, row->GetField<int>(3));
    EXPECT_EQ(44, row->GetField<int>("id_plus_42"));
    DataRow expected = DataRow::Create<int, std::string_view, storage::date, int, std::string_view>(2, "Igor", storage::date(1986, 9, 9), 2 + 42, "rogI");
    EXPECT_EQ(row->GetRow(), expected);
}

TEST_F(TestAddColumn, ViewAddSingleColumn) {
    DataContainerPtr view_with_column = storage->View("name", "id")->AddColumn(FieldDesc::String("nickname"), [](const DataFieldAccessor& row) {
        std::stringstream ss;
        ss << row.GetField<std::string_view>("name") << " " << "molodec";
        return ss.str();
    });
    EXPECT_EQ(3, view_with_column->FieldsCount());
    DataFieldAccessorPtr row = view_with_column->GetRow(1);

    EXPECT_EQ("Igor molodec", row->GetField<std::string_view>(2));
    EXPECT_EQ("Igor molodec", row->GetField<std::string_view>("nickname"));
    DataRow expected = DataRow::Create<std::string_view, int, std::string_view>("Igor", 2, "Igor molodec");
    EXPECT_EQ(row->GetRow(), expected);
}

TEST_F(TestAddColumn, ViewAddTwoColumns) {
    auto add_nickname_cb = [](const DataFieldAccessor& row) {
        std::stringstream ss;
        ss << row.GetField<std::string_view>("name") << " " << "molodec";
        return ss.str();
    };
    auto negative_id_cb = [](const DataFieldAccessor& row) {
        return -row.GetField<int>("id");
    };

    DataContainerPtr view_with_column2 =
        storage->View("name", "id")->
        AddColumn(FieldDesc::String("nickname"), add_nickname_cb)->
        AddColumn(FieldDesc::Int("neg_id"), negative_id_cb);
    EXPECT_EQ(4, view_with_column2->FieldsCount());
    
    DataFieldAccessorPtr row = view_with_column2->GetRow(1);

    EXPECT_EQ(-2, row->GetField<int>(3));
    EXPECT_EQ(-2, row->GetField<int>("neg_id"));
    DataRow expected = DataRow::Create<std::string_view, int, std::string_view, int>("Igor", 2, "Igor molodec", -2);
    EXPECT_EQ(row->GetRow(), expected);
}

TEST_F(TestAddColumn, SetAddSingleColumn) {
    DataSetPtr dataSet = DataSet::Create(storage->View("id", "birthday"));
    DataContainerPtr set_with_column = dataSet->AddColumn(FieldDesc::Double("year"), [](const DataFieldAccessor& row) -> double {
        return row.GetField<storage::date>("birthday").year();
        });
    EXPECT_EQ(3, set_with_column->FieldsCount());
    DataFieldAccessorPtr row = set_with_column->GetRow(1);

    EXPECT_EQ(1986, row->GetField<double>(2));
    EXPECT_EQ(1986, row->GetField<double>("year"));
    DataRow expected = DataRow::Create<int, storage::date, double>(2, storage::date(1986, 9, 9), 1986);
    EXPECT_EQ(row->GetRow(), expected);
}

TEST_F(TestAddColumn, SetAddTwoColumns) {
    DataSetPtr dataSet = DataSet::Create(storage->View("id", "birthday"));
    auto birthday_year_cb = [](const DataFieldAccessor& row) -> double {
        return row.GetField<storage::date>("birthday").year();
    };
    auto birthday_as_string_cb = [](const DataFieldAccessor& row) {
        storage::date b = row.GetField<storage::date>("birthday");
        std::stringstream ss;
        ss << b.year() << "." << b.month() << "." << b.day();
        return ss.str();
    };
    DataContainerPtr set_with_column2 =
        dataSet->
        AddColumn(FieldDesc::Double("year"), birthday_year_cb)->
        AddColumn(FieldDesc::String("birthday_str"), birthday_as_string_cb);

    EXPECT_EQ(4, set_with_column2->FieldsCount());
    DataFieldAccessorPtr row = set_with_column2->GetRow(1);

    EXPECT_EQ("1986.9.9", row->GetField<std::string_view>(3));
    EXPECT_EQ("1986.9.9", row->GetField<std::string_view>("birthday_str"));
    DataRow expected = DataRow::Create<int, storage::date, double, std::string_view>(2, storage::date(1986, 9, 9), 1986, "1986.9.9");
    EXPECT_EQ(row->GetRow(), expected);
}
