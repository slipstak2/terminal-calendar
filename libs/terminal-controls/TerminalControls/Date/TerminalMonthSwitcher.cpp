#include "TerminalMonthSwitcher.h"

TerminalMonthSwitcher::TerminalMonthSwitcher(int month, TerminalCoord position)
    : month(month)
    , TerminalBorderControl("", position, TerminalSize{.height = 8, .width = 24})
{

}