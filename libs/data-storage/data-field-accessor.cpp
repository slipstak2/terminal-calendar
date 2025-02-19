#include "data-field-accessor.h"
#include "data-row.h"

#include <sstream>

DataRow DataFieldAccessorBase::GetRow(const std::vector<size_t>& fields_num) const {
    DataRow result(fields_num.size());
    for (size_t field_num : fields_num) {
        result.AddFieldData(GetFieldData(field_num));
    }
    return result;
}

std::string DataFieldAccessorBase::ToString() {
    std::stringstream ss;
    size_t fields_count = FieldsCount();
    if (fields_count) {
        ss << GetFieldData(0);
    }
    for (size_t field_num = 1; field_num < fields_count; ++field_num) {
        ss << ", " << GetFieldData(field_num);
    }
    return ss.str();
}
