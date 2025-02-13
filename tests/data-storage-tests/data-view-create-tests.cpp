#include "pch.h"

#include "data-storage.h"
#include "data-view.h"
#include "data-set.h"

class TestDataViewCreate : public ::testing::Test {

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

DataStoragePtr TestDataViewCreate::storage;

TEST_F(TestDataViewCreate, AllFields_SameOrder) {

    auto view = DataView::Create(storage);
    EXPECT_EQ(view->RowsCount(), 3);

    for (size_t row_num = 0; row_num < view->RowsCount(); ++row_num) {
        DataRow view_row = view->GetRow(row_num)->GetRow();
        EXPECT_EQ(3, view_row.FieldsCount());
        EXPECT_EQ(view_row, storage->GetDataRow(row_num));
    }
}

TEST_F(TestDataViewCreate, AllField_OtherOrder) {

    auto view = DataView::Create(storage, 2, 0, 1);
    EXPECT_EQ(view->RowsCount(), 3);

    DataRow view_row = view->GetRow(0)->GetRow();
    EXPECT_EQ(3, view_row.FieldsCount());

    EXPECT_EQ(view_row.GetFieldType(0), FieldType::DATE);    // 2
    EXPECT_EQ(view_row.GetFieldType(1), FieldType::INT);     // 0
    EXPECT_EQ(view_row.GetFieldType(2), FieldType::STRING);  // 1

    EXPECT_EQ(storage::date(1996, 12, 25), view_row.GetField<storage::date>(0));
    EXPECT_EQ(1, view_row.GetField<int>(1));
    EXPECT_EQ("Dan4ick", view_row.GetField<std::string_view>(2));
}

TEST_F(TestDataViewCreate, AllField_SingleField) {

    auto view = DataView::Create(storage, 1);
    EXPECT_EQ(view->RowsCount(), 3);

    DataRow view_row = view->GetRow(0)->GetRow();
    EXPECT_EQ(1, view_row.FieldsCount());

    EXPECT_EQ(view_row.GetFieldType(0), FieldType::STRING);  // 1
    EXPECT_EQ("Dan4ick", view_row.GetField<std::string_view>(0));
}

void TestTwoFields(DataViewPtr view) {
    EXPECT_EQ(view->RowsCount(), 3);

    DataRow view_row = view->GetRow(0)->GetRow();
    EXPECT_EQ(2, view_row.FieldsCount());

    EXPECT_EQ(view_row.GetFieldType(0), FieldType::STRING);  // 1
    EXPECT_EQ(view_row.GetFieldType(1), FieldType::INT);     // 0

    EXPECT_EQ("Dan4ick", view_row.GetField<std::string_view>(0));
    EXPECT_EQ(1, view_row.GetField<int>(1));
}

TEST_F(TestDataViewCreate, AllField_TwoFields) {
    auto view_by_idx = DataView::Create(storage, 1, 0);
    TestTwoFields(view_by_idx);

    auto view_by_name = DataView::Create(storage, "name", "id");
    TestTwoFields(view_by_name);
}

TEST_F(TestDataViewCreate, FromDataStorage) {
    auto view_by_idx = storage->View(1, 0);
    TestTwoFields(view_by_idx);

    auto view_by_name = storage->View("name", "id");
    TestTwoFields(view_by_name);

    auto view_by_mix1 = DataView::Create(storage, 1, "id");
    TestTwoFields(view_by_mix1);

    auto view_by_mix2 = DataView::Create(storage, "name", 0);
    TestTwoFields(view_by_mix2);
}

TEST_F(TestDataViewCreate, FromDataView) {
    auto view = storage->View(1, 0)->View();
    EXPECT_EQ(2, view->FieldsCount());
}

TEST_F(TestDataViewCreate, FromDataViewSingleField) {
    auto view = storage->View(1, 0)->View(0);
    EXPECT_EQ(1, view->FieldsCount());
    DataRow row = view->GetRow(0)->GetRow();
    DataRow expect_row = DataRow::Create<std::string_view>("Dan4ick");
    EXPECT_EQ(row, expect_row);
}

TEST_F(TestDataViewCreate, FromDataSet) {
    auto dataSet = DataSet::Create(storage->View(1, 0));
    auto view = dataSet->View();
    EXPECT_EQ(2, view->FieldsCount());
}

TEST_F(TestDataViewCreate, FromDataSetSingleField) {
    auto dataSet = DataSet::Create(storage->View(1, 0));
    auto view = dataSet->View(1);
    EXPECT_EQ(1, view->FieldsCount());

    DataRow row = view->GetRow(2)->GetRow(); // Masha
    DataRow expect_row = DataRow::Create<int>(3);
    EXPECT_EQ(row, expect_row);
}
