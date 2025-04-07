#include "TerminalMonthSwitcher.h"
#include "TerminalLabel.h"
#include "TerminalGrid.h"
#include "TerminalCheckBox.h"

#include <chrono>

void fillRow(DataRow& row, std::chrono::year_month_day& d) {
    int wbeg = std::chrono::weekday(d).iso_encoding() - 1;

    const std::string empty = "";
    for (int wday = 0; wday < wbeg; ++wday) {
        row.SetField<std::string>(0, empty);
    }
    for (int wday = wbeg; wday < 7; ++wday) {
        if (d.ok()) {
            auto day = static_cast<unsigned>(d.day());
            std::string cur = std::to_string(day);
            if (cur.size() < 2) {
                cur = " " + cur;
            }
            row.SetField(wday, cur);

            d = d.year() / d.month() / (d.day() + std::chrono::days{ 1 });
        }
        else {
            row.SetField(wday, empty);
        }
    }
}

TerminalMonthSwitcher::TerminalMonthSwitcher(int year, int month, TerminalCoord position)
    : TerminalBorderControl("", position, TerminalSize{.height = DefaultHeight(), .width = DefaultWidth()})
    , provider(monthsDataSet, month)

{
    short offset = (Width() - (short)provider.Get().size()) / 2;
    auto monthLabel = TerminalLabel::Create(provider.Get(), TerminalCoord{ .row = 0, .col = offset });
    AddControlOnBorder(monthLabel);

    std::vector<Utf8String> header = { "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс" };
    auto storage = DataStorage::Create(
        FieldDesc::String("monday"),
        FieldDesc::String("tuesday"),
        FieldDesc::String("wednesday"),
        FieldDesc::String("thursday"),
        FieldDesc::String("friday"),
        FieldDesc::String("saturday"),
        FieldDesc::String("sunday")
    );
        
    auto date = std::chrono::year(year) / std::chrono::month(month + 1) / std::chrono::day(1);
    while (date.ok()) {
        DataRow& row = storage->AddEmptyRow();
        fillRow(row, date);
    }
        
    auto grid = TerminalGrid::Create(header, storage, TerminalCoord{.row = 0, .col = 0});
    AddControl(grid);

    for (int row_num = 0; row_num < grid->GetStorage()->RowsCount(); ++row_num) {
        auto rowMark = TerminalCheckBox::Create("", TerminalCoord{ .row = ONE + ONE + (short)row_num, .col = 0 });
        rowMark->mouseOverColor = rowMark->selectedColor;

        AddControlOnBorder(rowMark);

    }
}