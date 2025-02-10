#include "common.h"
#include "data-set.h"
#include "data-view.h"

DataSetRow::DataSetRow(const DataSetPtr dataSet, const size_t row_num)
    : dataSet(dataSet)
    , viewRow(dataSet->view->GetRow(row_num))
    , storageRow(dataSet->storage->GetRow(row_num))
{}

size_t DataSetRow::FieldsCount() const {
    return dataSet->FieldsCount();
}

std::string_view DataSetRow::GetFieldName(size_t field_num) const {
    if (field_num < dataSet->view->FieldsCount()) {
        return dataSet->view->GetFieldName(field_num);
    } else {
        return dataSet->storage->GetFieldName(field_num - dataSet->view->FieldsCount());
    }
}

DataRow DataSetRow::GetRow() {
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
    if (size_t field_index = dataSet->view->GetFieldIndex(field_name); field_index != -1) {
        return field_index;
    }
    if (size_t field_index = dataSet->storage->GetFieldIndex(field_name); field_index != -1) {
        return dataSet->view->FieldsCount() + field_index;
    }
    return -1;
}

DataFieldAccessorPtr DataSet::GetRow(size_t row_num) {
    return std::make_shared<DataSetRow>(shared_from_this(), row_num);
}
