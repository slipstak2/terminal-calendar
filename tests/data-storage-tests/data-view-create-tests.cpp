#include "pch.h"

#include "data-storage.h"
#include "data-view.h"

class TestDataViewCreate : public ::testing::Test {

public:
    static void SetUpTestCase() {
        storage = DataStorage::Create({ CreateField::Int("id"), CreateField::String("name"), CreateField::Date("birthday") });
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

    for (size_t row_idx = 0; row_idx < view->RowsCount(); ++row_idx) {
        DataRow view_row = view->GetRow(row_idx);
        EXPECT_EQ(3, view_row.FieldsCount());
        EXPECT_EQ(view_row, storage->GetRow(row_idx));
    }
}

TEST_F(TestDataViewCreate, AllField_OtherOrder) {

    auto view = DataView::Create(storage, { 2, 0, 1});
    EXPECT_EQ(view->RowsCount(), 3);

    DataRow view_row = view->GetRow(0);
    EXPECT_EQ(3, view_row.FieldsCount());

    EXPECT_EQ(view_row.GetRawField(0).header.type, FieldType::DATE);    // 2
    EXPECT_EQ(view_row.GetRawField(1).header.type, FieldType::INT);     // 0
    EXPECT_EQ(view_row.GetRawField(2).header.type, FieldType::STRING);  // 1

    EXPECT_EQ(storage::date(1996, 12, 25), view_row.GetField<storage::date>(0));
    EXPECT_EQ(1, view_row.GetField<int>(1));
    EXPECT_EQ("Dan4ick", view_row.GetField<std::string_view>(2));
}

TEST_F(TestDataViewCreate, AllField_SingleField) {

    auto view = DataView::Create(storage, { 1 });
    EXPECT_EQ(view->RowsCount(), 3);

    DataRow view_row = view->GetRow(0);
    EXPECT_EQ(1, view_row.FieldsCount());

    EXPECT_EQ(view_row.GetRawField(0).header.type, FieldType::STRING);  // 1
    EXPECT_EQ("Dan4ick", view_row.GetField<std::string_view>(0));
}

void TestTwoFields(DataViewPtr view) {
    EXPECT_EQ(view->RowsCount(), 3);

    DataRow view_row = view->GetRow(0);
    EXPECT_EQ(2, view_row.FieldsCount());

    EXPECT_EQ(view_row.GetRawField(0).header.type, FieldType::STRING);  // 1
    EXPECT_EQ(view_row.GetRawField(1).header.type, FieldType::INT);     // 0

    EXPECT_EQ("Dan4ick", view_row.GetField<std::string_view>(0));
    EXPECT_EQ(1, view_row.GetField<int>(1));
}

TEST_F(TestDataViewCreate, AllField_TwoFields) {
    auto view_by_idx = DataView::Create(storage, BY_IDX(1, 0));
    TestTwoFields(view_by_idx);

    auto view_by_name = DataView::Create(storage, BY_NAME( "name", "id" ));
    TestTwoFields(view_by_name);
}

TEST_F(TestDataViewCreate, FromDataStorage) {
    auto view_by_idx = storage->View(BY_IDX( 1, 0 ));
    TestTwoFields(view_by_idx);

    auto view_by_name = storage->View(BY_NAME( "name", "id" ));
    TestTwoFields(view_by_name);
}