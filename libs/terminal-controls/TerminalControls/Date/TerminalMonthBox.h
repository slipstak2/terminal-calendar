#pragma once

#include "TerminalBorderControl.h"
#include "DataProviders/ListDataProvider.h"

class TerminalMonthBox : public TerminalBorderControl {
public:
    DECLARE_KIND(TerminalBorderControl, TerminalControl::Kind::MONTH_BOX)

    DECLARE_CREATE(TerminalMonthBox)

    TerminalMonthBox(int year, int month, TerminalCoord position);

public:
    static short DefaultHeight() { return 1 + 5 + 2 + 1; };
    static short DefaultWidth() { return 2 * 7 + 8 + 2; };

protected:
    const Utf8String& monthStr;
};