#include "TerminalMonthBox.h"
#include "TerminalLabel.h"
#include "TerminalMonthGrid.h"
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

TerminalMonthBox::TerminalMonthBox(int year, int month, TerminalCoord position)
    : TerminalBorderControl("", position, TerminalSize{.height = DefaultHeight(), .width = DefaultWidth()})
    , monthStr(monthsDataSet->operator[](month))

{
    short offset = (Width() - (short)monthStr.size()) / 2;
    auto monthCheckBox = TerminalCheckBox::Create(monthStr, TerminalCoord{ .row = 0, .col = offset }, false);
    monthCheckBox->selectedColor = FontColor::Cyan;
    monthCheckBox->noSelectedColor = FontColor::Default;
    monthCheckBox->mouseOverColor = FontColor::Brightcyan;
    monthCheckBox->GetLabelButton()->SetFontColor(monthCheckBox->noSelectedColor);
    AddControlOnBorder(monthCheckBox);

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
        
    auto grid = TerminalMonthGrid::Create(header, storage, TerminalCoord{.row = 0, .col = 0});
    AddControl(grid);

    std::vector<TerminalCheckBoxPtr> rowsCheckBoxes;
    for (int row_num = 0; row_num < grid->GetStorage()->RowsCount(); ++row_num) {
        auto rowHeader = TerminalCheckBox::Create("", TerminalCoord{ .row = ONE + ONE + (short)row_num, .col = 0 });
        rowHeader->mouseOverColor = rowHeader->selectedColor;
        rowHeader->AddOnChangedCallback([row_num, grid](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
            grid->SetSelectedFullRow(row_num, isChecked, ctx.isCtrl);
        });
        AddControlOnBorder(rowHeader);
        rowsCheckBoxes.push_back(rowHeader);
    }
    grid->SetRowsCheckBoxes(std::move(rowsCheckBoxes));
    monthCheckBox->AddOnChangedCallback([grid](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
        grid->SetSelectedFull(isChecked, ctx.isCtrl);
    });
}