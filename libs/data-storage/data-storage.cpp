#include "data-storage.h"
#include "data-view.h"
#include "data-storage.h"

DataStorageRow::DataStorageRow(const DataStoragePtr storage, size_t row_num)
    : storage(storage)
    , row(storage->GetDataRow(row_num))
{}

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

    row_dummy.AddFieldType(fd.type);
}

size_t DataStorageRow::FieldsCount() const{
    return row.FieldsCount();
}

const FieldData& DataStorageRow::GetFieldData(size_t field_num) const {
    return row.GetFieldData(field_num);
}

size_t DataStorageRow::GetFieldIndex(const std::string_view field_name) const {
    return storage->GetFieldIndex(field_name);
}

std::string_view DataStorageRow::GetFieldName(size_t field_num) const {
    return storage->GetFieldName(field_num);
}

DataRow DataStorageRow::GetRow() {
    return row;
}

DataFieldAccessorPtr DataStorage::GetRow(size_t row_num) {
    return std::make_shared<DataStorageRow>(shared_from_this(), row_num);
}

size_t DataStorage::GetFieldIndex(size_t field_index) const {
    return field_index;
}

size_t DataStorage::GetFieldIndex(const std::string_view field_name) const {
    return ds_fields_mapping.at(field_name);
}

std::string_view DataStorage::GetFieldName(size_t field_num) const {
    return ds_fields_desc[field_num].name;
}

DataRow& DataStorage::AddEmptyRow() {
    rows.emplace_back(row_dummy);
    return rows.back();
}

const DataRow& DataStorage::GetDataRow(size_t row_num) const {
    return rows[row_num];
}

size_t DataStorage::RowsCount() const {
    return rows.size();
}

bool DataStorage::Empty() const {
    return rows.empty();
}

size_t DataStorage::FieldsCount() const {
    return ds_fields_desc.size();
}

const FieldDesc& DataStorage::Field(size_t idx) {
    return ds_fields_desc[idx];
}