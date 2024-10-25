#include "TerminalListView.h"
#include "TerminalLabel.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
{
    formatSettings.backgroundColor = BackgroundColor::Black;
    formatSettings.fontColor = FontColor::Yellow;
}

void TerminalListView::AddItem(std::string value) {
    provider.AddItem(value);

    auto label = TerminalLabel::Create(Utf8String(value), TerminalCoord{});
    AddControl(label);
    items.push_back(label);
}

void TerminalListView::FlushSelf() {
    for (int i = 0; i < items.size(); ++i) {
        items[i]->SetPosition({ .row = (short)i, .col = 0 });
    }

    for (int row = 0; row < Height(); ++row) {
        for (int col = 0; col < Width(); ++col) {
            data[row][col] = CreateCell('M');
        }
    }

}