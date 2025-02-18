#include "common.h"
#include "data-set.h"
#include "data-view.h"

#include <numeric> // iota

DataSetRow::DataSetRow(const DataSetPtr dataSet, const size_t row_num)
    : dataSet(dataSet)
    , viewRow(dataSet->view->GetRow(row_num))
    , storageRow(dataSet->storage->GetRow(row_num))
{}

size_t DataSetRow::FieldsCount() const {
    return dataSet->FieldsCount();
}

std::string_view DataSetRow::GetFieldName(size_t field_num) const {
    return dataSet->GetFieldName(field_num);
}

DataRow DataSetRow::GetRow() const {
    DataRow result(FieldsCount());
    for (size_t field_num = 0; field_num < viewRow->FieldsCount(); ++field_num) {
        result.AddFieldData(viewRow->GetFieldData(field_num));
    }
    for (size_t field_num = 0; field_num < storageRow->FieldsCount(); ++field_num) {
        result.AddFieldData(storageRow->GetFieldData(field_num));
    }
    return result;
}

const FieldData& DataSetRow::GetFieldData(size_t field_num) const {
    if (field_num < viewRow->FieldsCount()) {
        return viewRow->GetFieldData(field_num);
    } else {
        return storageRow->GetFieldData(field_num - viewRow->FieldsCount());
    }
}

size_t DataSetRow::GetFieldIndex(const std::string_view field_name) const {
    return dataSet->GetFieldIndex(field_name);
}

DEFINITIONS_VIEW(DataSet)

size_t DataSet::RowsCount() const {
    return view->RowsCount();
}

DataFieldAccessorPtr DataSet::GetRow(size_t row_num) {
    return std::make_shared<DataSetRow>(shared_from_this(), row_num);
}

size_t DataSet::GetFieldIndex(const std::string_view field_name) const {
    if (size_t field_index = view->GetFieldIndex(field_name); field_index != -1) {
        return field_index;
    }
    if (size_t field_index = storage->GetFieldIndex(field_name); field_index != -1) {
        return view->FieldsCount() + field_index;
    }
    return -1;
}

std::string_view DataSet::GetFieldName(size_t field_num) const {
    if (field_num < view->FieldsCount()) {
        return view->GetFieldName(field_num);
    }
    else {
        return storage->GetFieldName(field_num - view->FieldsCount());
    }
}

size_t DataSet::FieldsCount() const {
    return view->FieldsCount() + storage->FieldsCount();
}


DataContainerPtr DataSet::AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) {
    storage->AddFieldDesc(fd);
    for (size_t row_num = 0; row_num < view->RowsCount(); ++row_num) {
        DataRow& storage_row = storage->rows[row_num];
        DataSetRow dataset_row(shared_from_this(), row_num);
        storage_row.AddFieldData(FieldData(fd.type, add_column_cb(dataset_row)));
    }
    return shared_from_this();
}

DataContainerPtr DataSet::Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) {
    std::vector<size_t> rows_select_id;
    for (size_t row_num = 0; row_num < RowsCount(); ++row_num) {
        DataSetRow dataset_row(shared_from_this(), row_num);
        if (select_cb(dataset_row)) {
            rows_select_id.push_back(row_num);
        }
    }
    std::vector<size_t> fields_num(FieldsCount());
    std::iota(fields_num.begin(), fields_num.end(), 0);
    return DataView::Create(shared_from_this(), fields_num, rows_select_id);
}
