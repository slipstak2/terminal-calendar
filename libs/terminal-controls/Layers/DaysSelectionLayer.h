#pragma once

#include "SelectionLayer.h"

class DaysSelectionLayer : public SelectionLayer {
public:
    bool Select(TerminalControl* first, TerminalControl* last) override;
};