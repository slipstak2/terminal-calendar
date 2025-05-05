#pragma once

#include "TerminalCompositeControl.h"
#include "TerminalGridCell.h"
#include "data-storage.h"

#include "BorderFormat/BorderFormat.h"
#include "GridCellFormatter.h"

struct ColumnInfo {
    short width = 2;
};

class TerminalGrid : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::GRID)

    DECLARE_CREATE(TerminalGrid)

    TerminalGrid(const std::vector<Utf8String>& header, DataContainerPtr container, TerminalCoord position);
    void SetBorderVisible(bool isVisible);

    const DataContainerPtr GetContainer() const;
    
    void SetRowsCheckBoxes(std::vector<TerminalCheckBoxPtr>&& rowsCheckBoxes);
    void SetTitleCheckBox(TerminalCheckBoxPtr titleCheckBox);

    void SetSelectedFull(bool isSelected);
    void SetSelectedFullRow(size_t row, bool isSelected, bool isForce);
    void SetSelectedFullCol(size_t col, bool isSelected, bool isForce);

    template<typename ApplyFn>
    void ApplyForRow(size_t row, const ApplyFn& fn) {
        size_t cols = cells.empty() ? 0 : cells[0].size();
        for (size_t col = 0; col < cols; ++col) {
            if (cells[row][col]) {
                fn(col, cells[row][col]);
            }
        }
    }

    template<typename ApplyFn>
    void ApplyForCol(size_t col, const ApplyFn& fn) {
        for (size_t row = 0; row < cells.size(); ++row) {
            if (cells[row][col]) {
                fn(row, cells[row][col]);
            }
        }
    }

    void AddOnCellSelectedCallback(GridCellSelectedCallback selectedCallback);

public:
    void FinilizeSelectedCell(size_t row, size_t col);
protected:
    void FinilizeSelectedFull();
    void FinilizeSelectedRows();
    void FinilizeSelectedCols();

    void FinilizeSelectedRow(size_t row);
    void FinilizeSelectedCol(size_t col);
    void FinilizeSelectedTitle();

protected:
    void InitHeader();
    virtual void InitData() = 0;

    void FlushRowBorder(short row);
    void FlushSelf() override;

protected:
    std::vector<Utf8String> header;
    DataContainerPtr container;
    std::vector<ColumnInfo> columns;

protected:
    GridCellFormatter formatter;

protected:
    std::vector<std::vector<TerminalGridCellPtr>> cells;
    std::vector<TerminalCheckBoxPtr> colsCheckBoxes;
    std::vector<TerminalCheckBoxPtr> rowsCheckBoxes;
    TerminalCheckBoxPtr titleCheckBox;

protected:
    FormatSettings borderFormatSettings = FormatSettings::Default;
    BorderFormat borderFormat = BorderFormat::Default;

    FontColor selectedHeaderColor = FontColor::Yellow;
    FontColor noSelectedHeaderColor = FontColor::Brightblack;

protected:
    std::vector<GridCellSelectedCallback> cellSelectedCallbacks;
};
