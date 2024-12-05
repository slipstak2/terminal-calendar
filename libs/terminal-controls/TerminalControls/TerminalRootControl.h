#pragma once

#include "TerminalControl.h"
#include "TerminalWindow.h"

class TerminalRootControl : public TerminalControl {
public:
    DECLARE_CREATE(TerminalRootControl)

    TerminalRootControl(short rows, short cols);
    bool MoveToTop(const TerminalControl* window);
    void FlushSelf() override;
};