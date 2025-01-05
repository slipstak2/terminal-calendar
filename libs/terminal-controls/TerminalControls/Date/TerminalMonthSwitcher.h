#pragma once

#include "TerminalBorderControl.h"
#include "DataProviders/ListDataProvider.h"

class TerminalMonthSwitcher : public TerminalBorderControl {
public:
    DECLARE_KIND(TerminalBorderControl, TerminalControl::Kind::MONTH_SWITCHER)

    DECLARE_CREATE(TerminalMonthSwitcher)

    TerminalMonthSwitcher(int month, TerminalCoord position);

public:
    static short DefaultHeight() { return 1 + 5 + 2; };
    static short DefaultWidth() { return 2 * 7 + 8 + 2; };

protected:
    ListDataProvider provider;
};