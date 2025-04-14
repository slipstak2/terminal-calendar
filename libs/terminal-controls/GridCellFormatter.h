#pragma once

#include "TextFormat/TextStyle.h"
#include "TerminalGridCell.h"

struct GridCellFormat {
    TextStyle textStyle = TextStyle::Default;

    RGB backgroundColor = RGB::DefaultBackground;
    RGB backgroundSelectedColor = RGB::DefaultBackground;

    FontColor fontColor = FontColor::Default;
    RGB fontSelectedColor = RGB::DefaultFont;

    static GridCellFormat defaultCellFormat;
    static GridCellFormat weekEndCellFormat;
};


class GridCellFormatter {
public:
    void Apply(TerminalGridCell* cell);
};