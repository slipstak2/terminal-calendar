#include "data-storage.h"
#include "data-view.h"

StringHeapStorage DataRow::stringStorage;


DataViewPtr DataStorage::View() {
    return DataView::Create(shared_from_this());
}
