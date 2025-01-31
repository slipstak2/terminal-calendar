#include "data-storage.h"
#include "data-view.h"

StringHeapStorage DataRow::stringStorage;

DataViewPtr DataStorage::View(std::vector<size_t>&& fields_idx) {
    return DataView::Create(shared_from_this(), std::move(fields_idx));
}
