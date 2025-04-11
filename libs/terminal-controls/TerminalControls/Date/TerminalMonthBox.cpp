#include "TerminalMonthBox.h"
#include "TerminalLabel.h"
#include "TerminalMonthGrid.h"
#include "TerminalCheckBox.h"

#include <chrono>

void fillRow(DataRow& row, std::chrono::year_month_day& d) {
    int wbeg = std::chrono::weekday(d).iso_encoding() - 1;

    storage::date empty;
    for (int wday = 0; wday < wbeg; ++wday) {
        row.SetField(0, empty);
    }
    for (int wday = wbeg; wday < 7; ++wday) {
        if (d.ok()) {
            storage::date value(d);
            row.SetField(wday, value);

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
        FieldDesc::Date("monday"),
        FieldDesc::Date("tuesday"),
        FieldDesc::Date("wednesday"),
        FieldDesc::Date("thursday"),
        FieldDesc::Date("friday"),
        FieldDesc::Date("saturday"),
        FieldDesc::Date("sunday")
    );

    auto date = std::chrono::year(year) / std::chrono::month(month + 1) / std::chrono::day(1);
    while (date.ok()) {
        DataRow& row = storage->AddEmptyRow();
        fillRow(row, date);
    }
        
    grid = TerminalMonthGrid::Create(header, storage, TerminalCoord{.row = 0, .col = 0});
    AddControl(grid);

    std::vector<TerminalCheckBoxPtr> rowsCheckBoxes;
    for (int row_num = 0; row_num < grid->GetStorage()->RowsCount(); ++row_num) {
        auto rowHeader = TerminalCheckBox::Create("", TerminalCoord{ .row = ONE + ONE + (short)row_num, .col = 0 });
        rowHeader->mouseOverColor = rowHeader->selectedColor;
        rowHeader->AddOnChangedCallback([row_num, this](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
            grid->SetSelectedFullRow(row_num, isChecked, ctx.isCtrl);
        });
        AddControlOnBorder(rowHeader);
        rowsCheckBoxes.push_back(rowHeader);
    }
    grid->SetRowsCheckBoxes(std::move(rowsCheckBoxes));
    grid->SetTitleCheckBox(monthCheckBox);
    monthCheckBox->AddOnChangedCallback([this](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
        grid->SetSelectedFull(isChecked, ctx.isCtrl);
    });
}

void TerminalMonthBox::SetSelectionLayer(SelectionLayer* selectionLayer) {
    grid->SetSelectionLayer(selectionLayer);
}


void TerminalMonthBox::AddOnCellSelectedCallback(GridCellSelectedCallback selectedCallback) {
    grid->AddOnCellSelectedCallback(selectedCallback);
}