#include "TerminalListView.h"
#include "TerminalLabel.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
{
    provider.AddChangeItemsCallback([this](const ListViewDataProvider* provider, size_t curItemsCount, size_t prvItemsCount) {
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
    ChangeOffset(0);
    return isRemove;
}

bool TerminalListView::NeedScroll() {
    return Height() < TotalItems();
}

bool TerminalListView::HasUp() {
    return viewOffset > 0;
}

bool TerminalListView::HasDown() {
    return viewOffset < MaxViewOffset();
}

void TerminalListView::AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback) {
    changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
}

void TerminalListView::AddChangeOffsetCallback(TerminalListViewChangedOffsetCallback changeOffsetCallback) {
    changeOffsetCallbacks.push_back(std::move(changeOffsetCallback));
}

void TerminalListView::OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount) {
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, curItemsCount, prvItemsCount);
    }
}

void TerminalListView::OnChangeOffset(int curOffset, int prvOffset) {
    for (auto& callback : changeOffsetCallbacks) {
        callback(this, curOffset, prvOffset);
    }
}

int TerminalListView::TotalItems() const{
    return provider.TotalItems();
}

bool TerminalListView::ChangeOffset(int delta) {
    int initViewOffset = viewOffset;
    viewOffset += delta;
    viewOffset = NormalizeOffset(viewOffset);
    if (viewOffset != initViewOffset) {
        OnChangeOffset(viewOffset, initViewOffset);
        return true;
    }
    return false;
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
    for (size_t i = 0; i < slice.size(); ++i) {
        auto& text = slice[i];
        controls[i]->As<TerminalLabel>()->SetText(text);
    }
    for (size_t i = slice.size(); i < Height(); ++i) {
        controls[i]->As<TerminalLabel>()->SetText("");
    }
}