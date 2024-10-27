#include "TerminalListView.h"
#include "TerminalLabel.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
{
    formatSettings.backgroundColor = BackgroundColor::Black;
    formatSettings.fontColor = FontColor::Yellow;

    for (short row = 0; row < Height(); ++row) {
        auto label = TerminalLabel::Create(TerminalCoord{ .row = row }, TerminalSize{.height = 1, .width = size.width});
        AddControl(label);
    }
}

void TerminalListView::AddItem(std::string value) {
    provider.AddItem(value);
}

void TerminalListView::FlushSelf() {
    auto slice = provider.GetLast(Height());
    for (int i = 0; i < slice.size(); ++i) {
        auto& text = slice[i];
        controls[i]->As<TerminalLabel>()->SetText(text);
    }
}