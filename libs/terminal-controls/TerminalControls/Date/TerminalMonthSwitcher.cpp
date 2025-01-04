#include "TerminalMonthSwitcher.h"
#include "TerminalLabel.h"

TerminalMonthSwitcher::TerminalMonthSwitcher(int month, TerminalCoord position)
    : month(month)
    , TerminalBorderControl("", position, TerminalSize{.height = 8, .width = 24})
{
    auto monthLabel = TerminalLabel::Create("Март", TerminalCoord{ .row = 0, .col = 9 });
    AddControlOnBorder(monthLabel);

}