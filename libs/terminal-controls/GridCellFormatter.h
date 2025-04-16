#pragma once

#include "TextFormat/TextStyle.h"
#include "TerminalGridCell.h"

struct GridCellFormat {
    TextStyle textStyle = TextStyle::Default;

    RGB backgroundColor = RGB::DefaultBackground;
    RGB backgroundSelectedColor = RGB::DefaultBackground;

    RGB fontColor = RGB::DefaultFont;
    RGB fontSelectedColor = RGB::DefaultFont;

    void ApplyCurrentDaySettings();

    static GridCellFormat defaultCellFormat;
    static GridCellFormat weekEndCellFormat;
    static GridCellFormat currentDayCellFormat;
};


class GridCellFormatter {
public:
    void Apply(TerminalGridCell* cell);
};