#include "GridCellFormatter.h"

GridCellFormat defaultCellFormat{
    .textStyle = TextStyle::Default,
    .backgroundColor = RGB::DefaultBackground,
    .backgroundSelectedColor = RGB::Brightcyan,
    .fontColor = FontColor::Default,
    .fontSelectedColor = RGB::Blue
};

GridCellFormat weekEndCellFormat{
    .textStyle = TextStyle::Bold,
    .backgroundColor = RGB::DefaultBackground,
    .backgroundSelectedColor = RGB::DarkRed,
    .fontColor = FontColor::Red,
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
        sender->SetBackgroundColor(cellFormat.backgroundSelectedColor);
    }
    else {
        sender->SetFontColor(cellFormat.fontColor);
        sender->SetBackgroundColor(cellFormat.backgroundColor);
    }
}