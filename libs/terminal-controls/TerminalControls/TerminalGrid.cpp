#include "TerminalGrid.h"
#include "TerminalCheckBox.h"
#include "TerminalGridCell.h"
#include "data-view.h"

using defer = std::shared_ptr<void>;

TerminalGrid::TerminalGrid(const std::vector<Utf8String>& header, DataContainerPtr container, TerminalCoord position)
    : TerminalCompositeControl(position)
    , header(header)
    , container(container) {

    short totalWidth = 0;
    columns.resize(header.size());
    for (size_t i = 0; i < header.size(); ++i) {
        columns[i].width = header[i].size();
        totalWidth += columns[i].width;
    }
    colsCheckBoxes.resize(header.size());
    cells.resize(container->RowsCount(), std::vector<TerminalGridCellPtr>(container->FieldsCount(), nullptr));

    InitHeader();
    //InitData();

    SetBorderVisible(false);
    SetSize(TerminalSize{ .height = ONE + (short)container->RowsCount(), .width = totalWidth + ((short)columns.size() + 1)});
}

void TerminalGrid::SetBorderVisible(bool isVisible) {
    borderFormatSettings.textStyle = isVisible ? TextStyle::Default : TextStyle::Conceal;
}

const DataContainerPtr TerminalGrid::GetContainer() const {
    return container;
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
    short col = 0;
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
    for (short row_num = 0; row_num < container->RowsCount(); ++row_num) {
        FlushRowBorder(ONE + row_num);
    }
}

void TerminalGrid::AddOnCellSelectedCallback(GridCellSelectedCallback selectedCallback) {
    cellSelectedCallbacks.push_back(selectedCallback);
}