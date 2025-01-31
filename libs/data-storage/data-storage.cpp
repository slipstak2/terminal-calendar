#include "data-storage.h"
#include "data-view.h"

StringHeapStorage DataRow::stringStorage;


DataViewPtr DataStorage::View() {
    return DataView::Create(shared_from_this());
}

DataViewPtr DataStorage::View(const std::vector<size_t>& fields_idx) {
    return DataView::Create(shared_from_this(), fields_idx);
}

DataViewPtr DataStorage::View(const std::vector<std::string_view>& fields_name) {
    return DataView::Create(shared_from_this(), fields_name);
}

std::vector<size_t> DataStorage::GetFieldsIdx(const std::vector<std::string_view>& fields_name) {
    std::vector<size_t> fields_idx;
    fields_idx.resize(fields_name.size());

    for (size_t i = 0; i < fields_idx.size(); ++i) {
        fields_idx[i] = ds_fields_mapping[fields_name[i]];
    }

    return fields_idx;
}
