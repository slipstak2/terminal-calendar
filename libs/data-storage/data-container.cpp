#include "data-container.h"
#include "data-row.h"
#include "data-view.h"

std::vector<size_t> GenRowsNum(size_t rows_count) {
    std::vector<size_t> rows_num(rows_count);
    std::iota(rows_num.begin(), rows_num.end(), 0);
    return rows_num;
}

std::vector<size_t> GenFieldsNum(size_t fields_count) {
    std::vector<size_t> fields_num(fields_count);
    std::iota(fields_num.begin(), fields_num.end(), 0);
    return fields_num;
}

std::string DataContainer::ToString() {
    std::stringstream ss;
    for (size_t row_num = 0; row_num < RowsCount(); ++row_num) {
        ss << GetRow(row_num)->GetRow().ToString() << std::endl;
    }
    return ss.str();
}


DataViewPtr DataContainer::Limit(const size_t limit) {
    return DataView::Create(
        SelfPtr(),
        GenFieldsNum(FieldsCount()),
        GenRowsNum(std::min(RowsCount(), limit))
    );
}