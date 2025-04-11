#include "GridCellFormatter.h"

void GridCellFormatter::Apply(TerminalGridCell* sender) {
    if (sender->IsSelected()) {
        sender->SetTextStyle(TextStyle::Default);

        sender->SetFontColor(RGB::Blue);
        if (sender->SelectedWeight() == 1) {
            sender->SetBackgroundColor(RGB::Brightcyan);
        }
        else {
            sender->SetBackgroundColor(RGB::DarkBrightcyan);
        }
    }
    else {
        storage::date d = sender->GetData();
        int dayNum = d.weekday().c_encoding();
        if (dayNum == 6 || dayNum == 0) { // Суббота + Воскресенье
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