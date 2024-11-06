#include "TerminalListView.h"
#include "TerminalLabel.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
{
    formatSettings.backgroundColor = BackgroundColor::Black;
    formatSettings.fontColor = FontColor::Yellow;

    provider.AddChangeItemsCallback([this](const ListViewDataProvider* provider, int curItemsCount, int prvItemsCount) {
        OnChangeItemsCount(curItemsCount, prvItemsCount);
        });

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

bool TerminalListView::RemoveLastItem() {
    bool isRemove = provider.RemoveLastItem();
    viewOffset = NormalizeOffset(viewOffset);
    return isRemove;
}

void TerminalListView::AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback) {
    changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
}

void TerminalListView::OnChangeItemsCount(int curItemsCount, int prvItemsCount)
{
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, curItemsCount, prvItemsCount);
    }
}

int TerminalListView::TotalItems() const{
    return provider.TotalItems();
}

bool TerminalListView::ChangeOffset(int delta) {
    int initViewOffset = viewOffset;
    viewOffset += delta;
    viewOffset = NormalizeOffset(viewOffset);
    return viewOffset != initViewOffset;
}

int TerminalListView::MaxViewOffset() {
    if (TotalItems() >= Height()) {
        return TotalItems() - Height();
    }
    return 0;
}

int TerminalListView::NormalizeOffset(int offset) {
    offset = std::max(0, offset);
    offset = std::min(offset, MaxViewOffset());
    return offset;
}

void TerminalListView::FlushSelf() {
    auto slice = provider.GetView(viewOffset, Height());
    for (int i = 0; i < slice.size(); ++i) {
        auto& text = slice[i];
        controls[i]->As<TerminalLabel>()->SetText(text);
    }
    for (int i = slice.size(); i < Height(); ++i) {
        controls[i]->As<TerminalLabel>()->SetText("");
    }
}