#include "TerminalGrid.h"
#include "TerminalCheckBox.h"
#include "TerminalGridCell.h"

TerminalGrid::TerminalGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position)
    : TerminalCompositeControl(position)
    , header(header)
    , storage(storage) {

    columns.resize(header.size());
    colsCheckBoxes.resize(header.size());
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

void TerminalGrid::SetRowsCheckBoxes(std::vector<TerminalCheckBoxPtr>&& rowsCheckBoxes) { 
    this->rowsCheckBoxes = std::move(rowsCheckBoxes); 
}

void TerminalGrid::SetSelectedFull(bool isSelected, bool isForce) {
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[row].size(); ++col) {
            if (cells[row][col]) {
                cells[row][col]->SetSelected(isSelected, isForce);
            }
        }
    }
    FinilizeSelected();
}

void TerminalGrid::FinilizeSelected() {
    FinilizeSelectedRows();
    FinilizeSelectedCols();
}

void TerminalGrid::FinilizeSelectedRows() {
    for (size_t row = 0; row < cells.size(); ++row) {
        FinilizeSelectedRow(row);
    }
}

void TerminalGrid::FinilizeSelectedCols() {
    for (size_t col = 0; col < cells[0].size(); ++col) {
        FinilizeSelectedCol(col);
    }
}

void TerminalGrid::SetSelectedFullCol(size_t col, bool isSelected, bool isForce) {
    for (size_t row = 0; row < cells.size(); ++row) {
        if (cells[row][col]) {
            cells[row][col]->SetSelected(isSelected, isForce);
        }
    }
    FinilizeSelectedRows();
}

void TerminalGrid::FinilizeSelectedRow(size_t row) {
    int total = 0;
    int selectedSingle = 0;
    int selectedCount = 0;
    for (size_t col = 0; col < cells[0].size(); ++col) {
        if (cells[row][col]) {
            total++;
            selectedCount += cells[row][col]->IsSelected();
            selectedSingle += (cells[row][col]->SelectedWeight() == 1 ? 1 : 0);
        }
    }
    
    if (total == selectedSingle && !rowsCheckBoxes[row]->GetChecked()) {
        rowsCheckBoxes[row]->SetChecked(true, false);
    }
    if (total != selectedCount && rowsCheckBoxes[row]->GetChecked()) {
        rowsCheckBoxes[row]->SetChecked(false, false);
    }
}

void TerminalGrid::FinilizeSelectedCol(size_t col) {
    int total = 0;
    int selectedSingle = 0;
    int selectedCount = 0;
    for (size_t row = 0; row < cells.size(); ++row) {
        if (cells[row][col]) {
            total++;
            selectedCount += cells[row][col]->IsSelected();
            selectedSingle += (cells[row][col]->SelectedWeight() == 1 ? 1 : 0);
        }
    }
    if (total == selectedSingle && !colsCheckBoxes[col]->GetChecked()) {
        colsCheckBoxes[col]->SetChecked(true, false);
    }
    if (total != selectedCount && colsCheckBoxes[col]->GetChecked()) {
        colsCheckBoxes[col]->SetChecked(false, false);
    }
}

void TerminalGrid::SetSelectedFullRow(size_t row, bool isSelected, bool isForce) {
    size_t cols = cells.empty() ? 0 : cells[0].size();
    for (size_t col = 0; col < cols; ++col) {
        if (cells[row][col]) {
            cells[row][col]->SetSelected(isSelected, isForce);
        }
    }
    FinilizeSelectedCols();
}

void TerminalGrid::InitHeader() {
    short col = 1;
    for (short column = 0; column < columns.size(); ++column) {
        auto columnCheckBox = TerminalCheckBox::Create(header[column], TerminalCoord{.row = 0, .col = col}, false);
        columnCheckBox->AddOnChangedCallback([this, column](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
            SetSelectedFullCol(column, isChecked, ctx.isCtrl);
        });
        colsCheckBoxes[column] = columnCheckBox;
        AddControl(columnCheckBox);

        col += columns[column].width + 1;
    }
}


void TerminalGrid::InitData() {
    for (size_t row_num = 0; row_num < storage->RowsCount(); ++row_num) {
        short col = 1;
        auto row = storage->GetRow(row_num);

        for (size_t field_num = 0; field_num < row->FieldsCount(); ++field_num) {
            storage::date d = row->GetField<storage::date>(field_num);
            if (d.ok()) {
                auto dayValue = static_cast<unsigned>(d.day());
                std::string day = std::to_string(dayValue);
                if (day.size() < 2) {
                    day = " " + day;
                }
                auto dayCell = TerminalGridCell::Create(day, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                dayCell->SetData(d);
                dayCell->SetGridPosition(row_num, field_num);
                dayCell->AddClickCallback([dayCell, this](const MouseContext& ctx) {
                    FinilizeSelectedRow(dayCell->GridRow());
                    FinilizeSelectedCol(dayCell->GridCol());
                    return true;
                });
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
