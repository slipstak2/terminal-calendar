#pragma once

#include "TerminalBorderControl.h"

class TerminalMonthSwitcher : public TerminalBorderControl {
public:
    DECLARE_KIND(TerminalBorderControl, TerminalControl::Kind::MONTH_SWITCHER)

    DECLARE_CREATE(TerminalMonthSwitcher)

    TerminalMonthSwitcher(int month, TerminalCoord position);

protected:
    int month = 1;
};