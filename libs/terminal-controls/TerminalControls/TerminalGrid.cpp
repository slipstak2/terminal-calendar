#include "TerminalGrid.h"
#include "TerminalCheckBox.h"
#include "TerminalGridCell.h"

using defer = std::shared_ptr<void>;

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

void TerminalGrid::SetTitleCheckBox(TerminalCheckBoxPtr titleCheckBox) {
    this->titleCheckBox = titleCheckBox;
}

void TerminalGrid::SetSelectedFull(bool isSelected) {
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[row].size(); ++col) {
            if (cells[row][col]) {
                cells[row][col]->SetSelected(isSelected);
            }
        }
    }
    FinilizeSelectedFull();
}


void TerminalGrid::FinilizeSelectedCell(size_t row, size_t col) {
    FinilizeSelectedRow(row);
    FinilizeSelectedCol(col);
    FinilizeSelectedTitle();
}
void TerminalGrid::FinilizeSelectedFull() {
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

void TerminalGrid::FinilizeSelectedRow(size_t row) {
    int total = 0;
    int selectedCount = 0;
    for (size_t col = 0; col < cells[0].size(); ++col) {
        if (cells[row][col]) {
            total++;
            selectedCount += cells[row][col]->IsSelected();
        }
    }
    
    if (total == selectedCount && !rowsCheckBoxes[row]->GetChecked()) {
        rowsCheckBoxes[row]->SetChecked(true, false);
    }
    if (total != selectedCount && rowsCheckBoxes[row]->GetChecked()) {
        rowsCheckBoxes[row]->SetChecked(false, false);
    }
}

void TerminalGrid::FinilizeSelectedCol(size_t col) {
    int total = 0;
    int selectedCount = 0;
    for (size_t row = 0; row < cells.size(); ++row) {
        if (cells[row][col]) {
            total++;
            selectedCount += cells[row][col]->IsSelected();
        }
    }
    if (total == selectedCount && !colsCheckBoxes[col]->GetChecked()) {
        colsCheckBoxes[col]->SetChecked(true, false);
    }
    if (total != selectedCount && colsCheckBoxes[col]->GetChecked()) {
        colsCheckBoxes[col]->SetChecked(false, false);
    }
}

void TerminalGrid::SetSelectedFullRow(size_t row, bool isSelected, bool isForce) {
    defer _(nullptr, [this](...) { FinilizeSelectedTitle(); });

    if (!isSelected) {
        bool isOK = false;
        ApplyForRow(row, [&](size_t col, TerminalGridCellPtr cell) {
            if (cell->IsSelected()) {
                if (!colsCheckBoxes[col]->GetChecked() || isForce) {
                    isOK = true;
                    cell->SetSelected(isSelected);
                    FinilizeSelectedCol(col);
                }
            }
        });
        if (isOK) {
            return;
        }
    }
    ApplyForRow(row, [&](size_t col, TerminalGridCellPtr cell) {
        cell->SetSelected(isSelected);
        FinilizeSelectedCol(col);
    });
}

void TerminalGrid::SetSelectedFullCol(size_t col, bool isSelected, bool isForce) {
    defer _(nullptr, [this](...) { FinilizeSelectedTitle(); });

    if (!isSelected) {
        bool isOK = false;
        ApplyForCol(col, [&](size_t row, TerminalGridCellPtr cell) {
            if (cell->IsSelected()) {
                if (!rowsCheckBoxes[row]->GetChecked() || isForce) {
                    isOK = true;
                    cell->SetSelected(isSelected);
                    FinilizeSelectedRow(row);
                }
            }
        });
        if (isOK) {
            return;
        }
    }
    ApplyForCol(col, [&](size_t row, TerminalGridCellPtr cell) {
        cell->SetSelected(isSelected);
        FinilizeSelectedRow(row);
    });
    
}

void TerminalGrid::FinilizeSelectedTitle() {
    int total = 0;
    int selectedCount = 0;
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[0].size(); ++col) {
            if (cells[row][col]) {
                total++;
                selectedCount += cells[row][col]->IsSelected();
            }
        }
    }
    if (total == selectedCount && !titleCheckBox->GetChecked()) {
        titleCheckBox->SetChecked(true, false);
    }
    if (total != selectedCount && titleCheckBox->GetChecked()) {
        titleCheckBox->SetChecked(false, false);
    }
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
                dayCell->SetGridCellFormatter(&formatter);
                dayCell->AddClickCallback([dayCell, this](const MouseContext& ctx) {
                    FinilizeSelectedCell(dayCell->GridRow(), dayCell->GridCol());
                    return true;
                });
                dayCell->AddOnSelectedCallback([this](TerminalGridCell* sender) {
                    for (auto& cellSelectedCallback : cellSelectedCallbacks) {
                        cellSelectedCallback(sender);
                    }
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

void TerminalGrid::AddOnCellSelectedCallback(GridCellSelectedCallback selectedCallback) {
    cellSelectedCallbacks.push_back(selectedCallback);
}