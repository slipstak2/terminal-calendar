#include "pch.h"

#include "data-storage.h"
#include "data-view.h"

class TestDataViewSort : public ::testing::Test {

public:
    static void SetUpTestCase() {
        storage = DataStorage::Create(
            CreateFieldDesc::Int("id"), 
            CreateFieldDesc::String("name"),
            CreateFieldDesc::Date("birthday")
        );
        storage->AddRow<int, std::string_view, storage::date>(1, "Dan4ick", storage::date(1996, 12, 25));
        storage->AddRow<int, std::string_view, storage::date>(2, "Masha", storage::date(1986, 12, 2));
        storage->AddRow<int, std::string_view, storage::date>(3, "Igor", storage::date(1986, 9, 9));
        
    }
public:
    static DataStoragePtr storage;
};

DataStoragePtr TestDataViewSort::storage;

TEST_F(TestDataViewSort, ByIdRev) {

    auto view = storage->View();
    EXPECT_EQ(view->RowsCount(), 3);

}
