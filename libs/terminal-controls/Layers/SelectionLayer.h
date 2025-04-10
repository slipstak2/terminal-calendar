#pragma once

#include "TerminalControls/TerminalControl.h"
#include <vector>

class SelectionLayer {
public:
    SelectionLayer() = default;
    virtual ~SelectionLayer() = default;

    void AddControl(TerminalControl* control);
    virtual bool Select(TerminalControl* first, TerminalControl* last) = 0;
    virtual bool StopSelect() = 0;

protected:
    std::vector<TerminalControl*> controls;
};