#pragma once
#include "TerminalControl.h"

class TerminalCompositeControl : public TerminalControl {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::COMPOSITE_CONTROL)

    TerminalCompositeControl(TerminalCoord position);
    TerminalCompositeControl(TerminalCoord position, TerminalSize size);
    void FlushSelf() override;
    void AddControl(TerminalControlPtr control) override;
};