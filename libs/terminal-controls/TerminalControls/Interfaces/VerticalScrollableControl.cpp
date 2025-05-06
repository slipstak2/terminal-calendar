#include <algorithm>

#include "VerticalScrollableControl.h"

bool VerticalScrollableControl::NeedScroll() const {
    return ViewItems() < TotalItems();
}

int VerticalScrollableControl::MaxViewOffset() const {
    return std::max(TotalItems() - ViewItems(), 0);
}

bool VerticalScrollableControl::CanScrollUp() const {
    return viewOffset > 0;
}

bool VerticalScrollableControl::CanScrollDown() const {
    return viewOffset < MaxViewOffset();
}

int VerticalScrollableControl::NormalizeOffset(int offset) const {
    offset = std::max(0, offset);
    offset = std::min(offset, MaxViewOffset());
    return offset;
}

bool VerticalScrollableControl::ChangeViewOffset(int delta) {
    return SetViewOffset(viewOffset + delta);
}

int VerticalScrollableControl::GetViewOffset() const {
    return viewOffset;
}

bool VerticalScrollableControl::SetViewOffset(int newOffset) {
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


bool VerticalScrollableControl::SetSelectedRow(int rowNum) {
    if (rowNum != -1) {
        if (rowNum < 0 || rowNum >= TotalItems()) {
            return false;
        }
    }
    if (selectedRow == rowNum) {
        return false;
    }
    int initSelectedRow = selectedRow;
    selectedRow = rowNum;
    OnChangeSelectedRow(initSelectedRow);
    return true;
}

int VerticalScrollableControl::GetSelectedRow() {
    return selectedRow;
}

bool VerticalScrollableControl::IsSelectedRowInView() {
    if (selectedRow == -1) {
        return false;
    }
    return viewOffset <= selectedRow && selectedRow < viewOffset + ViewItems();
}
bool VerticalScrollableControl::NavigateOnSelectedRow() {
    if (selectedRow == -1) {
        return false;
    }
    SetViewOffset(selectedRow - ViewItems() / 2);
    return false;
}

bool VerticalScrollableControl::MoveSelectedRow(bool isUp) {
    if (!IsSelectedRowInView()) {
        NavigateOnSelectedRow();
    }
    bool isChange = false;
    if (isUp) {
        if (selectedRow != 0) {
            isChange |= SetSelectedRow(selectedRow - 1);
            if (!IsSelectedRowInView()) {
                isChange |= ChangeViewOffset(-1);
            }
        }
    } else {
        isChange |= SetSelectedRow(selectedRow + 1);
        if (!IsSelectedRowInView()) {
            isChange |= ChangeViewOffset(1);
        }
    }
    return isChange;
}