#include "data-storage.h"
#include "data-view.h"
#include "data-set.h"
#include "defines.h"

#include <numeric>
#include <algorithm>
#include <unordered_set>


DataStorageRow::DataStorageRow(const DataStoragePtr storage, size_t row_num)
    : storage(storage)
    , row(storage->GetDataRow(row_num))
{}

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

DataRow DataStorageRow::GetRow() const {
    return row;
}

DEFINITIONS_VIEW(DataStorage)

DataViewPtr DataStorage::Unique() {
    return nullptr;
}

DataViewPtr DataStorage::Unique(size_t fn0) {
    return nullptr;
}

DataFieldAccessorPtr DataStorage::GetRow(size_t row_num) {
    return std::make_shared<DataStorageRow>(shared_from_this(), row_num);
}

size_t DataStorage::GetFieldIndex(const std::string_view field_name) const {
    auto it = ds_fields_mapping.find(field_name);
    if (it == ds_fields_mapping.end()) {
        return -1;
    }
    return it->second;
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

DataContainerPtr DataStorage::AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) {
    return View()->AddColumn(fd, add_column_cb);
}

DataContainerPtr DataStorage::Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) {
    std::vector<size_t> rows_num_selected;
    for (size_t row_num = 0; row_num < RowsCount(); ++row_num) {
        DataStorageRow storage_row(shared_from_this(), row_num);
        if (select_cb(storage_row)) {
            rows_num_selected.push_back(row_num);
        }
    }
    std::vector<size_t> fields_num(FieldsCount());
    std::iota(fields_num.begin(), fields_num.end(), 0);
    return DataView::Create(shared_from_this(), fields_num, rows_num_selected);
}

DataContainerPtr DataStorage::Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) {
    std::vector<size_t> rows_num_sorted(RowsCount());
    std::iota(rows_num_sorted.begin(), rows_num_sorted.end(), 0);
    std::stable_sort(rows_num_sorted.begin(), rows_num_sorted.end(), [&](const size_t lhs, const size_t rhs) {
        DataStorageRow lhs_row(shared_from_this(), lhs);
        DataStorageRow rhs_row(shared_from_this(), rhs);
        return cmp_cb(lhs_row, rhs_row);
        });
    std::vector<size_t> fields_num(FieldsCount());
    std::iota(fields_num.begin(), fields_num.end(), 0);
    return DataView::Create(shared_from_this(), std::move(fields_num), std::move(rows_num_sorted));
}