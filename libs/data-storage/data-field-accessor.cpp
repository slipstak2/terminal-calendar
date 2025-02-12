#include "data-field-accessor.h"
#include "data-row.h"

DataRow DataFieldAccessorBase::GetRow(const std::vector<size_t>& fields_num) const {
    DataRow result(fields_num.size());
    for (size_t field_num : fields_num) {
        result.AddFieldData(GetFieldData(field_num));
    }
    return result;
}
