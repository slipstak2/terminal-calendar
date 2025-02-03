#include "pch.h"

#include "data-storage.h"
#include "data-view.h"

class TestDataViewSort : public ::testing::Test {

public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(CreateField::Int("id"), CreateField::String("name"), CreateField::Date("birthday"));
        storage->AddRow<int, std::string_view, storage::date>(1, "Dan4ick", storage::date(1996, 12, 25));
        storage->AddRow<int, std::string_view, storage::date>(2, "Igor", storage::date(1986, 9, 9));
        storage->AddRow<int, std::string_view, storage::date>(3, "Masha", storage::date(1986, 12, 2));
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestDataViewSort::storage;

TEST_F(TestDataViewSort, ByIdRev) {

    auto view = storage->View();
    EXPECT_EQ(view->RowsCount(), 3);

}
