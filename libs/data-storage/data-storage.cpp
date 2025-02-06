#include "data-storage.h"
#include "data-view.h"

StringHeapStorage DataRow::stringStorage;


DataViewPtr DataStorage::View() {
    return DataView::Create(shared_from_this());
}

void DataStorage::AddFieldDesc(const FieldDesc& fd) {
    if (ds_fields_mapping.contains(fd.name)) {
        std::string message = "Field with name " + std::string(fd.name) + " already exists";
        throw std::runtime_error(message);
    }

    ds_fields_desc.push_back(fd);
    ds_fields_mapping[ds_fields_desc.back().name] = ds_fields_desc.size() - 1;

    row_dummy.fields.push_back({ .type = fd.type }); // TODO: row_dummy.AddFieldDesc()
}

void DataStorage::AddFieldData(size_t row_idx, const FieldData& field_data) {
    rows[row_idx].AddFieldData(field_data);
}





size_t DataStorageRow::FieldsCount() const{
    return row.FieldsCount();
}

FieldData DataStorageRow::GetFieldData(int field_num) const {
    return row.GetFieldData(field_num);
}

FieldData DataStorageRow::GetFieldData(const char* field_name) const{
    return row.GetFieldData(storage->GetFieldIndex(field_name));
}

std::string_view DataStorageRow::GetFieldName(size_t field_num) const {
    return storage->Field(field_num).name;
}

DataRow DataStorageRow::GenRow() {
    return row;
}