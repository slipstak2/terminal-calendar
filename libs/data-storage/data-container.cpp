#include "data-container.h"

std::vector<size_t> GenFieldsNum(size_t fields_count) {
    std::vector<size_t> fields_num(fields_count);
    std::iota(fields_num.begin(), fields_num.end(), 0);
    return fields_num;
}