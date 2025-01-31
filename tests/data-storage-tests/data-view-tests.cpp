#include "pch.h"

#include <tuple>
#include "data-storage.h"


TEST(TestDataView, Create) {

    auto storage = DataStorage::Create({ CreateField::Int("id"), CreateField::String("name"), CreateField::Date("birthday") });
    storage->CreateRow<int, std::string_view, storage::date>(1, "Dan4ick", storage::date(1996, 12, 25));
    storage->CreateRow<int, std::string_view, storage::date>(2, "Igor", storage::date(1986, 9, 9));
    storage->CreateRow<int, std::string_view, storage::date>(2, "Masha", storage::date(1986, 12, 2));
    EXPECT_EQ(storage->RowsCount(), 3);
}