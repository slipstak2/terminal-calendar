#include "TerminalGridEx.h"
#include "data-view.h"
#include "TerminalLabel.h"


TerminalGridEx::TerminalGridEx(const std::vector<Utf8String>& header, DataContainerPtr container, TerminalCoord position)
    : TerminalGrid(header, container, position)
{
    InitData();
    //SetBorderVisible(true);
}

void TerminalGridEx::InitData() {
    for (size_t row_num = 0; row_num < container->RowsCount(); ++row_num) {
        short col = 1;
        auto row = container->GetRow(row_num);

        for (size_t field_num = 0; field_num < row->FieldsCount(); ++field_num) {
            std::string value;
            switch (row->GetFieldType(field_num)) {
            case FieldType::INT:
                value = std::to_string(row->GetField<int>(field_num));
                break;
            case FieldType::STRING:
                value = row->GetField<std::string_view>(field_num);
                break;
            case FieldType::DATE:
                value = row->GetField<storage::date>(field_num).to_string();
                break;
            }
            auto cell = TerminalGridCell::Create(value, columns[field_num].width, TerminalCoord{.row = ONE + (short)row_num, .col = col});
            cell->SetGridPosition(row_num, field_num);
            AddControl(cell);

            col += columns[field_num].width + 1;
        }
    }
}