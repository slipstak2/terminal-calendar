#include "TerminalMonthSwitcher.h"
#include "TerminalLabel.h"

TerminalMonthSwitcher::TerminalMonthSwitcher(int month, TerminalCoord position)
    : TerminalBorderControl("", position, TerminalSize{.height = DefaultHeight(), .width = DefaultWidth()})
    , provider(monthDataSet, month)

{
    short offset = (Width() - provider.Get().size()) / 2;
    auto monthLabel = TerminalLabel::Create(provider.Get(), TerminalCoord{ .row = 0, .col = offset });
    AddControlOnBorder(monthLabel);

    auto headerLabel = TerminalLabel::Create(" Пн Вт Ср Чт Пт Сб Вс ", TerminalCoord{ .row = 0, .col = 0 });
    auto week0 =       TerminalLabel::Create("        1  2  3  4  5 ", TerminalCoord{ .row = 1, .col = 0 });
    auto week1 =       TerminalLabel::Create("  6  7  8  9 10 11 12 ", TerminalCoord{ .row = 2, .col = 0 });
    auto week2 =       TerminalLabel::Create(" 13 14 15 16 17 18 19 ", TerminalCoord{ .row = 3, .col = 0 });
    auto week3 =       TerminalLabel::Create(" 20 21 22 23 24 25 26 ", TerminalCoord{ .row = 4, .col = 0 });
    auto week4 =       TerminalLabel::Create(" 27 28 29 30 31       ", TerminalCoord{ .row = 5, .col = 0 });
    AddControl(headerLabel);
    AddControl(week0);
    AddControl(week1);
    AddControl(week2);
    AddControl(week3);
    AddControl(week4);
}