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

void TerminalListView::AddItem(const std::string& value) {
    bool incOffset = false;
    if (TotalItems() >= Height()) {
        if (viewOffset == MaxViewOffset()) {
            incOffset = true;
        }
    }
    provider.AddItem(value);
    if (incOffset) {
        ChangeOffset(1);
    }
}

int TerminalListView::TotalItems() const{
    return provider.TotalItems();
}

bool TerminalListView::ChangeOffset(int delta) {
    int initViewOffset = viewOffset;
    viewOffset += delta;
    viewOffset = std::max(0, viewOffset);
    viewOffset = std::min(viewOffset, MaxViewOffset());
    return viewOffset != initViewOffset;
}

int TerminalListView::MaxViewOffset() {
    return TotalItems() - Height();
}

void TerminalListView::FlushSelf() {
    auto slice = provider.GetView(viewOffset, Height());
    for (int i = 0; i < slice.size(); ++i) {
        auto& text = slice[i];
        controls[i]->As<TerminalLabel>()->SetText(text);
    }
}