#include "TerminalMonthSwitcher.h"
#include "TerminalLabel.h"
#include <chrono>

std::string getRow(std::string& s, std::chrono::year_month_day& d) {
    int wbeg = std::chrono::weekday(d).iso_encoding() - 1;
    s += " ";
    
    for (int wday = 0; wday < wbeg; ++wday) {
        s += "   ";
    }
    for (int wday = wbeg; wday < 7; ++wday) {
        if (d.ok()) {
            auto day = static_cast<unsigned>(d.day());
            std::string cur = std::to_string(day);
            if (cur.size() < 2) {
                s += ' ';
            }
            s += cur;

            d = d.year() / d.month() / (d.day() + std::chrono::days{ 1 });
        }
        else {
            s += "  ";
        }
        s += ' ';
    }

    s += " ";
    return s;
}

TerminalMonthSwitcher::TerminalMonthSwitcher(int year, int month, TerminalCoord position)
    : TerminalBorderControl("", position, TerminalSize{.height = DefaultHeight(), .width = DefaultWidth()})
    , provider(monthsDataSet, month)

{
    short offset = (Width() - (short)provider.Get().size()) / 2;
    auto monthLabel = TerminalLabel::Create(provider.Get(), TerminalCoord{ .row = 0, .col = offset });
    AddControlOnBorder(monthLabel);

    auto date = std::chrono::year(year) / std::chrono::month(month + 1) / std::chrono::day(1);
    auto date_end = std::chrono::year(year) / std::chrono::month(month + 1) / std::chrono::last;

    std::string s;
    s.reserve(DefaultWidth());
 

    auto headerLabel = TerminalLabel::Create(" Пн Вт Ср Чт Пт Сб Вс ", TerminalCoord{ .row = 0, .col = 0 });
    AddControl(headerLabel);
    short row_num = 1;
    while (date.ok()) {
        s.clear();
        getRow(s, date);
        auto weekLabel = TerminalLabel::Create(s, TerminalCoord{ .row = row_num++, .col = 0 });
        AddControl(weekLabel);
    }
}