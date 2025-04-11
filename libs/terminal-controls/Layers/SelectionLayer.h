#pragma once

#include "TerminalControls/TerminalControl.h"
#include <vector>

class SelectionLayer {
public:
    SelectionLayer() = default;
    virtual ~SelectionLayer() = default;

    void AddControl(TerminalControl* control);
    void Clear();
    virtual bool SelectStart(TerminalControl* begControl) = 0;
    virtual bool Select(TerminalControl* endControl) = 0;
    virtual bool SelectStop() = 0;

protected:
    std::vector<TerminalControl*> controls;
};