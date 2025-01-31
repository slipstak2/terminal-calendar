#include "pch.h"

#include <tuple>
#include "data-storage.h"
#include "data-view.h"


TEST(TestDataView, Create) {

    auto storage = DataStorage::Create({ CreateField::Int("id"), CreateField::String("name"), CreateField::Date("birthday") });
    storage->AddRow<int, std::string_view, storage::date>(1, "Dan4ick", storage::date(1996, 12, 25));
    storage->AddRow<int, std::string_view, storage::date>(2, "Igor", storage::date(1986, 9, 9));
    storage->AddRow<int, std::string_view, storage::date>(2, "Masha", storage::date(1986, 12, 2));

    auto view = DataView::Create(storage);
    EXPECT_EQ(view->RowsCount(), 3);

    for (size_t row_idx = 0; row_idx < view->RowsCount(); ++row_idx) {
        const DataRow& view_row = view->GetRow(row_idx);
        EXPECT_EQ(view_row, storage->GetRow(row_idx));
    }
}