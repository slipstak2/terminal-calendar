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
        OnChangeOffset(initViewOffset);
        return true;
    }
    return false;
}

void VerticalScrollableControl::OnChangeOffset(int prvOffset) {
    for (auto& changeOffsetCallback : changeOffsetCallbacks) {
        changeOffsetCallback(this, prvOffset);
    }
}

void VerticalScrollableControl::AddChangeOffsetCallback(ChangedOffsetCallback changeOffsetCallback) {
    changeOffsetCallbacks.push_back(std::move(changeOffsetCallback));
}

void VerticalScrollableControl::OnChangeSelectedRow(int prvSelectedRow) {
    for (auto& changeSelectedRowCallback : changeSelectedRowCallbacks) {
        changeSelectedRowCallback(this, prvSelectedRow);
    }
}

void VerticalScrollableControl::AddChangeSelectedRowCallback(ChangeSelectedRowCallback changeSelectedRowCallback) {
    changeSelectedRowCallbacks.push_back(std::move(changeSelectedRowCallback));
}

void VerticalScrollableControl::AddChangeItemsCountCallback(ChangedItemsCountCallback changeItemsCountCallback) {
    changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
}

void VerticalScrollableControl::OnChangeItemsCount(size_t prvItemsCount) {
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, prvItemsCount);
    }
}
