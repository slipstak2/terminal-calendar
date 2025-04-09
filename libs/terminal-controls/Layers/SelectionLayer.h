#pragma once

#include "TerminalControls/TerminalControl.h"
#include <vector>

class SelectionLayer {
public:
    SelectionLayer() = default;
    virtual ~SelectionLayer() = default;

    void AddControl(TerminalControlPtr control);
    virtual bool Select(TerminalControl* first, TerminalControl* last) = 0;

protected:
    std::vector<TerminalControlPtr> controls;
};