#pragma once

#include "TextFormat/TextStyle.h"
#include "TerminalGridCell.h"

class GridCellFormatter {
public:
    void Apply(TerminalGridCell* cell);
};