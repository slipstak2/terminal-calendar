#include "GridCellFormatter.h"

GridCellFormat defaultCellFormat{
    .textStyle = TextStyle::Default,
    .backgroundColor = RGB::DefaultBackground,
    .backgroundSelectedColor = RGB::Brightcyan,
    .backgroundDoubleSelectedColor = RGB::DarkBrightcyan,
    .fontColor = RGB::DefaultFont,
    .fontSelectedColor = RGB::Blue
};

GridCellFormat weekEndCellFormat{
    .textStyle = TextStyle::Bold,
    .backgroundColor = RGB::DefaultBackground,
    .backgroundSelectedColor = RGB::DarkRed,
    .backgroundDoubleSelectedColor = RGB::DarkDarkRed,
    .fontColor = RGB::Coral,
    .fontSelectedColor = RGB::BrightRed
};

void GridCellFormatter::Apply(TerminalGridCell* sender) {
    storage::date d = sender->GetData();
    int dayNum = d.weekday().c_encoding();
    bool isWeekend = dayNum == 6 || dayNum == 0;

    sender->SetTextStyle(isWeekend ? TextStyle::Bold : TextStyle::Default);

    GridCellFormat cellFormat = defaultCellFormat;
    if (isWeekend) {
        cellFormat = weekEndCellFormat;
    }

    sender->SetTextStyle(cellFormat.textStyle);

    if (sender->IsSelected()) {
        sender->SetFontColor(cellFormat.fontSelectedColor);
        if (sender->SelectedWeight() == 1) {
            sender->SetBackgroundColor(cellFormat.backgroundSelectedColor);
        } else {
            sender->SetBackgroundColor(cellFormat.backgroundDoubleSelectedColor);
        }
    }
    else {
        sender->SetFontColor(cellFormat.fontColor);
        sender->SetBackgroundColor(cellFormat.backgroundColor);
    }
}