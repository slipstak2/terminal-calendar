#pragma once

#include "TextFormat/TextStyle.h"
#include "TerminalGridCell.h"

struct GridCellFormat {
    TextStyle textStyle = TextStyle::Default;

    RGB backgroundColor = RGB::DefaultBackground;
    RGB backgroundSelectedColor = RGB::DefaultBackground;
    RGB backgroundDoubleSelectedColor = RGB::DefaultBackground;

    RGB fontColor = RGB::DefaultFont;
    RGB fontSelectedColor = RGB::DefaultFont;

    static GridCellFormat defaultCellFormat;
    static GridCellFormat weekEndCellFormat;
};


class GridCellFormatter {
public:
    void Apply(TerminalGridCell* cell);
};