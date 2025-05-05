#include <algorithm>

#include "VerticalScrollableControl.h"

bool VerticalScrollableControl::NeedScroll() const {
    return ViewItems() < TotalItems();
}

int VerticalScrollableControl::MaxViewOffset() const {
    return std::max(TotalItems() - ViewItems(), 0);
}

bool VerticalScrollableControl::HasUp() const {
    return viewOffset > 0;
}

bool VerticalScrollableControl::HasDown() const {
    return viewOffset < MaxViewOffset();
}

int VerticalScrollableControl::NormalizeOffset(int offset) const {
    offset = std::max(0, offset);
    offset = std::min(offset, MaxViewOffset());
    return offset;
}

bool VerticalScrollableControl::ChangeOffset(int delta) {
    return SetOffset(viewOffset + delta);
}

int VerticalScrollableControl::GetOffset() const {
    return viewOffset;
}

bool VerticalScrollableControl::SetOffset(int newOffset) {
    int initViewOffset = viewOffset;
    viewOffset = NormalizeOffset(newOffset);
    if (viewOffset != initViewOffset) {
        OnChangeOffset(viewOffset, initViewOffset);
        return true;
    }
    return false;
}

void VerticalScrollableControl::OnChangeOffset(int curOffset, int prvOffset) {
    for (auto& callback : changeOffsetCallbacks) {
        callback(this, curOffset, prvOffset);
    }
}

void VerticalScrollableControl::AddChangeOffsetCallback(TerminalListViewChangedOffsetCallback changeOffsetCallback) {
    changeOffsetCallbacks.push_back(std::move(changeOffsetCallback));
}