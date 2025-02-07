#include "pch.h"

#include "data-storage.h"
#include "data-view.h"
#include "data-set.h"

class TestDataSetCreate : public ::testing::Test {

public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(
            CreateFieldDesc::Int("id"),
            CreateFieldDesc::String("name"),
            CreateFieldDesc::Date("birthday")
        );
        storage->AddRow<int, std::string_view, storage::date>(1, "Dan4ick", storage::date(1996, 12, 25));
        storage->AddRow<int, std::string_view, storage::date>(2, "Igor", storage::date(1986, 9, 9));
        storage->AddRow<int, std::string_view, storage::date>(3, "Masha", storage::date(1986, 12, 2));
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestDataSetCreate::storage;

TEST_F(TestDataSetCreate, SameAsStorage) {

    auto dataSet = DataSet::Create(storage->View());
    EXPECT_EQ(dataSet->RowsCount(), 3);

    for (size_t row_num = 0; row_num < dataSet->RowsCount(); ++row_num) {
        DataRow view_row = dataSet->GetViewRow(row_num)->FullRow();
        EXPECT_EQ(3, view_row.FieldsCount());
        EXPECT_EQ(view_row, storage->GetDataRow(row_num));
    }
}

TEST_F(TestDataSetCreate, AddColumn) {
    auto dataSet = DataSet::Create(storage->View());

    /*
    view.Add(CreateField::Int("age", [view.FieldAccessor()](const DataRow& row){
        Now() - fieldAccessor(row, "birthday");
    }))
    */
    auto addColumnCb = [](auto accessor) {
        //accessor.GetField<storage::date>("birthday");
        return accessor.get();
        };
    EXPECT_EQ(3, dataSet->FieldsCount());
    dataSet->AddColumn(CreateFieldDesc::Int("age"), addColumnCb);
    EXPECT_EQ(4, dataSet->FieldsCount());

    //dataSet->GetRow(0).GetField("age") == 12
    // dataSet->GetRow(0).GetField(4) == 12;

    //dataSet->AddColumn<int>("age", addColumnCb);
}
