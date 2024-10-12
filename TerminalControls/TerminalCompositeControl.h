#pragma once
#include "TerminalControl.h"

class TerminalCompositeControl : public TerminalControl {
public:
    TerminalCompositeControl(TerminalCoord position);
    void FlushSelf() override;
    void AddControl(TerminalControlPtr control) override;
};