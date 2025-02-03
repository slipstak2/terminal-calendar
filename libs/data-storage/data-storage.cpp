#include "data-storage.h"
#include "data-view.h"

StringHeapStorage DataRow::stringStorage;


DataViewPtr DataStorage::View() {
    return DataView::Create(shared_from_this());
}

template<>
size_t DataStorage::GetFieldIndex(int field_index) const {
    return field_index;
}

size_t DataStorage::GetFieldIndex(const char* field_name) const {
    return ds_fields_mapping.at(field_name);
}
