#include "data-view.h"


DataViewRow::DataViewRow(const DataViewPtr view, size_t row_num) 
    : view(view)
    , row(view->GetRowFromStorage(row_num))
{}

size_t DataViewRow::FieldsCount() const {
    return view->FieldsCount();
}

std::string_view DataViewRow::GetFieldName(size_t field_num) const {
    return view->GetFieldName(field_num);
}

DataRow DataViewRow::GetRow() {
    return row.GetRow(view->fields_num);
}

const FieldData& DataViewRow::GetFieldData(size_t field_num)  const {
    return row.GetFieldData(view->fields_num[field_num]);
}

size_t DataViewRow::GetFieldIndex(const std::string_view field_name) const {
    return view->GetFieldIndex(field_name);
}

const DataRow& DataView::GetRowFromStorage(size_t row_num) {
    return storage->GetDataRow(row_num);
}