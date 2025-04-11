#include "GridCellFormatter.h"

void GridCellFormatter::Apply(TerminalGridCell* sender) {
    storage::date d = sender->GetData();
    int dayNum = d.weekday().c_encoding();
    bool isWeekend = dayNum == 6 || dayNum == 0;
    if (sender->IsSelected()) {
        sender->SetTextStyle(TextStyle::Default);

        sender->SetFontColor(RGB::Blue);
        if (sender->SelectedWeight() == 1) {
            sender->SetBackgroundColor(RGB::Brightcyan);
        }
        else {
            sender->SetBackgroundColor(RGB::DarkBrightcyan);
        }

        if (isWeekend) { 
            sender->SetTextStyle(TextStyle::Bold);
            sender->SetFontColor(RGB::BrightRed);
            sender->SetBackgroundColor(RGB::DarkRed);
        }
    }
    else {
        if (isWeekend) { 
            sender->SetTextStyle(TextStyle::Bold);
            sender->SetFontColor(FontColor::Red);
            sender->SetBackgroundColor(BackgroundColor::Default);
        }
        else {
            sender->SetFontColor(FontColor::Default);
            sender->SetBackgroundColor(BackgroundColor::Default);
        }
    }
}