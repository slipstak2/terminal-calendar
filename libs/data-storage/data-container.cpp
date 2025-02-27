#include "data-container.h"
#include "data-row.h"

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
