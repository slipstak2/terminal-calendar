#include "pch.h"

#include "data-set.h"
#include "utils.h"

class TestLimit : public ::testing::Test {
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

DataStoragePtr TestLimit::storage;

TEST_F(TestLimit, StorageLimit) {
    auto storage_first3 = storage->Limit(3);

    EXPECT_EQ(2, storage_first3->FieldsCount());
    EXPECT_EQ(3, storage_first3->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
        DataRow::Create<int, std::string_view>(2, "Igor"),
        DataRow::Create<int, std::string_view>(3, "Masha")
    };

    CHECK_EQ(expected, storage_first3);
}

TEST_F(TestLimit, ViewLimit) {
    auto view_first4 = storage->View()->Limit(4);

    EXPECT_EQ(2, view_first4->FieldsCount());
    EXPECT_EQ(4, view_first4->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
        DataRow::Create<int, std::string_view>(2, "Igor"),
        DataRow::Create<int, std::string_view>(3, "Masha"),
        DataRow::Create<int, std::string_view>(4, "Vera")
    };

    CHECK_EQ(expected, view_first4);
}

TEST_F(TestLimit, DataSetLimit) {
    auto dataset_first2 = DataSet::Create(storage->View())->Limit(2);

    EXPECT_EQ(2, dataset_first2->FieldsCount());
    EXPECT_EQ(2, dataset_first2->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
        DataRow::Create<int, std::string_view>(2, "Igor"),
    };

    CHECK_EQ(expected, dataset_first2);
}


TEST_F(TestLimit, StorageLimitTwice) {
    auto storage_first2 = storage->Limit(3)->Limit(2);

    EXPECT_EQ(2, storage_first2->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
        DataRow::Create<int, std::string_view>(2, "Igor"),
    };

    CHECK_EQ(expected, storage_first2);
}

TEST_F(TestLimit, ViewLimitTwice) {
    auto view_first1 = storage->View()->Limit(4)->Limit(1);

    EXPECT_EQ(1, view_first1->RowsCount());

    std::vector<DataRow> expected{
        DataRow::Create<int, std::string_view>(1, "Dan4ick"),
    };

    CHECK_EQ(expected, view_first1);
}

TEST_F(TestLimit, DataSetLimitTwice) {
    auto dataset_first0 = DataSet::Create(storage->View())->Limit(2)->Limit(0);

    EXPECT_EQ(0, dataset_first0->RowsCount());

    std::vector<DataRow> expected;

    CHECK_EQ(expected, dataset_first0);
}