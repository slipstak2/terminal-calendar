#include "TerminalGrid.h"
#include "TerminalCheckBox.h"
#include "TerminalGridCell.h"

TerminalGrid::TerminalGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position)
    : TerminalCompositeControl(position)
    , header(header)
    , storage(storage) {

    columns.resize(header.size());
    cells.resize(storage->RowsCount(), std::vector<TerminalGridCellPtr>(storage->FieldsCount(), nullptr));

    InitHeader();
    InitData();

    SetBorderVisible(false);
    SetSize(TerminalSize{ .height = ONE + (short)storage->RowsCount(), .width = 2 * 7 + 8});
}

void TerminalGrid::SetBorderVisible(bool isVisible) {
    borderFormatSettings.textStyle = isVisible ? TextStyle::Default : TextStyle::Conceal;
}

const DataStoragePtr TerminalGrid::GetStorage() const {
    return storage;
}

void TerminalGrid::SetSelectedFull(bool isSelected, bool isForce) {
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[row].size(); ++col) {
            if (cells[row][col]) {
                cells[row][col]->SetSelected(isSelected, isForce);
            }
        }
    }
}

void TerminalGrid::SetSelectedFullCol(size_t col, bool isSelected, bool isForce) {
    for (size_t row = 0; row < cells.size(); ++row) {
        if (cells[row][col]) {
            cells[row][col]->SetSelected(isSelected, isForce);
        }
    }
}

void TerminalGrid::SetSelectedFullRow(size_t row, bool isSelected, bool isForce) {
    size_t cols = cells.empty() ? 0 : cells[0].size();
    for (size_t col = 0; col < cols; ++col) {
        if (cells[row][col]) {
            cells[row][col]->SetSelected(isSelected, isForce);
        }
    }
}

void TerminalGrid::InitHeader() {
    short col = 1;
    for (short column = 0; column < columns.size(); ++column) {
        auto headerCheckBox = TerminalCheckBox::Create(header[column], TerminalCoord{.row = 0, .col = col}, false);
        headerCheckBox->AddOnChangedCallback([this, column](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
            SetSelectedFullCol(column, isChecked, ctx.isCtrl);
        });
        AddControl(headerCheckBox);

        col += columns[column].width + 1;
    }
}


void TerminalGrid::InitData() {
    for (size_t row_num = 0; row_num < storage->RowsCount(); ++row_num) {
        short col = 1;
        auto row = storage->GetRow(row_num);

        for (size_t field_num = 0; field_num < row->FieldsCount(); ++field_num) {
            std::string day(row->GetField<std::string_view>(field_num));
            if (!day.empty()) {
                auto dayCell = TerminalGridCell::Create(day, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                dayCell->SetGridPosition(row_num, field_num);
                AddControl(dayCell);
                cells[row_num][field_num] = dayCell;
            }
            else {
                std::string empty_cell(columns[field_num].width, ' ');
                auto emptyLabel = TerminalLabel::Create(empty_cell, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                AddControl(emptyLabel);
            }
            col += columns[field_num].width + 1;

        }
    }
}

void TerminalGrid::FlushRowBorder(short row) {
    short col = 0;

    data[row][col] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
    for (short column = 0; column < columns.size(); ++column) {
        col += columns[column].width + 1;
        data[row][col] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
    }
}

void TerminalGrid::FlushSelf() {
    FlushRowBorder(0);
    for (short row_num = 0; row_num < storage->RowsCount(); ++row_num) {
        FlushRowBorder(ONE + row_num);
    }
}
