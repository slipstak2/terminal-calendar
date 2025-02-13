#include "data-view.h"


DataViewRow::DataViewRow(const DataViewPtr view, size_t row_num) 
    : view(view)
    , row(view->GetRowFromParent(row_num))
{}

size_t DataViewRow::FieldsCount() const {
    return view->FieldsCount();
}

std::string_view DataViewRow::GetFieldName(size_t field_num) const {
    return view->GetFieldName(field_num);
}

DataRow DataViewRow::GetRow() const {
    return row->GetRow(view->fields_num);
}

const FieldData& DataViewRow::GetFieldData(size_t field_num)  const {
    return row->GetFieldData(view->fields_num[field_num]);
}

size_t DataViewRow::GetFieldIndex(const std::string_view field_name) const {
    return view->GetFieldIndex(field_name);
}

DEFINITIONS_VIEW(DataView)

size_t DataView::RowsCount() const {
    return rows_idx.size();
}

std::string_view DataView::GetFieldName(size_t field_num) const {
    return container->GetFieldName(fields_num[field_num]);
}

size_t DataView::GetFieldIndex(const std::string_view field_name) const {
    auto it = dv_fields_mapping.find(field_name);
    if (it == dv_fields_mapping.end()) {
        return -1;
    }
    return it->second;
}

DataFieldAccessorPtr DataView::GetRow(size_t row_num) {
    return std::make_shared<DataViewRow>(shared_from_this(), row_num);
}

DataFieldAccessorPtr DataView::GetRowFromParent(size_t row_num) {
    return container->GetRow(row_num);
}

size_t DataView::FieldsCount() const {
    return fields_num.size();
}

DataView::DataView(const DataContainerPtr container) : container(container) {
    InitAllRows();
    InitAllFields();
    InitFieldsMapping();
}

DataView::DataView(const DataContainerPtr container, std::vector<size_t> fields_num) 
    : container(container)
    , fields_num(std::move(fields_num)) {
    InitAllRows();
    InitFieldsMapping();
}

void DataView::InitAllRows() {
    size_t rows_cnt = container->RowsCount();
    rows_idx.resize(rows_cnt);
    for (size_t idx = 0; idx < rows_cnt; ++idx) {
        rows_idx[idx] = idx;
    }
}
void DataView::InitAllFields() {
    size_t fields_cnt = container->FieldsCount();
    fields_num.resize(fields_cnt);
    for (size_t field_num = 0; field_num < fields_cnt; ++field_num) {
        fields_num[field_num] = field_num;
    }
}
void DataView::InitFieldsMapping() {
    for (size_t field_num = 0; field_num < fields_num.size(); ++field_num) {
        dv_fields_mapping[container->GetFieldName(fields_num[field_num])] = field_num;
    }
}