#include "data-view.h"
#include "data-set.h"

#include <algorithm>
#include <numeric>


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

DataContainerPtr DataView::SelfPtr() {
    return shared_from_this();
}

DataSetPtr DataView::AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) {
    return DataSet::Create(shared_from_this())->AddColumn(fd, add_column_cb);
}

DataViewPtr DataView::Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) {
    std::vector<size_t> rows_num_selected;
    rows_num_selected.reserve(RowsCount());
    for (size_t row_num = 0; row_num < RowsCount(); ++row_num) {
        DataViewRow view_row(shared_from_this(), rows_num[row_num]);
        if (select_cb(view_row)) {
            rows_num_selected.push_back(rows_num[row_num]);
        }
    }
    rows_num_selected.shrink_to_fit();
    return DataView::Create(container, fields_num, std::move(rows_num_selected));
}

DataViewPtr DataView::Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) {
    std::vector<size_t> rows_num_sorted = GenRowsNum(0, RowsCount());
    std::stable_sort(rows_num_sorted.begin(), rows_num_sorted.end(), [&](const size_t lhs, const size_t rhs) {
        DataViewRow lhs_row(shared_from_this(), rows_num[lhs]);
        DataViewRow rhs_row(shared_from_this(), rows_num[rhs]);
        return cmp_cb(lhs_row, rhs_row);
        });
    return DataView::Create(shared_from_this(), GenFieldsNum(fields_num.size()), std::move(rows_num_sorted));
}

size_t DataView::RowsCount() const {
    return rows_num.size();
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
    return std::make_shared<DataViewRow>(shared_from_this(), rows_num[row_num]);
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

DataView::DataView(DataContainerPtr container, std::vector<size_t> fields_num, std::vector<size_t> rows_num)
    : container(container) 
    , fields_num(std::move(fields_num))
    , rows_num(std::move(rows_num)) {
    InitFieldsMapping();
}

void DataView::InitAllRows() {
    size_t rows_cnt = container->RowsCount();
    rows_num.resize(rows_cnt);
    for (size_t row_num = 0; row_num < rows_cnt; ++row_num) {
        rows_num[row_num] = row_num;
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
        dv_fields_mapping[container->GetFieldName(fields_num[field_num])] = field_num; // TODO
    }
}