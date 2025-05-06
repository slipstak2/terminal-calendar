#include "TerminalVerticalScroll.h"
#include "Interfaces/VerticalScrollableControl.h"


TerminalVerticalScroll::TerminalVerticalScroll(VerticalScrollableControlPtr listView, TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
    , listView(listView)
{
    backgroundCell = CreateBackgroundCell(' ');
}

bool TerminalVerticalScroll::IsDraggable() {
    return true;
}

bool TerminalVerticalScroll::TryDraggingStart(TerminalCoord absPosition) {
    TerminalCoord position = GetRelativePosition(absPosition);
    assert(position.col == 0);
    if (OffsetHeight() <= position.row && position.row < OffsetHeight() + ScrollHeight()) {
        if (isScrollDragging) {
            return false;
        }
        isScrollDragging = true;
        scrollFormatSettings = FormatSettings::ScrollDragging;
        return true;
    }
    return false;
}

bool TerminalVerticalScroll::TryDraggingStop() {
    if (isScrollDragging == false) {
        return false;
    }
    isScrollDragging = false;
    scrollFormatSettings = FormatSettings::ScrollDefault;
    return true;
}

bool TerminalVerticalScroll::TryDragging(TerminalCoord delta) {
    if (delta.row != 0) {
        return listView->ChangeViewOffset(delta.row * ItemsPerCell());
    }
    return false;
}

void TerminalVerticalScroll::FlushSelf() {
    int offsetH = OffsetHeight();
    int scrollH = ScrollHeight();

    for (int row = 0; row < offsetH; ++row) {
        data[row][0] = backgroundCell;
    }
    for (int row = offsetH; row < offsetH + scrollH; ++row) {
        data[row][0] = CreateScrollCell("█");
    }

    for (int row = offsetH + scrollH; row < Height(); ++row) {
        data[row][0] = backgroundCell;
    }
}

int TerminalVerticalScroll::ScrollHeight() {
    int viewItems = listView->ViewItems();
    int totalItems = listView->TotalItems();

    int totalScroll = Height();
    int scrollHeight = (int)round((double)viewItems * totalScroll / totalItems);
    return scrollHeight;
}

int TerminalVerticalScroll::OffsetHeight() {
    int viewItems = listView->ViewItems();
    int totalItems = listView->TotalItems();

    int offsetHeight = (int)round((double)viewItems * listView->GetViewOffset() / totalItems);
    offsetHeight = std::min(offsetHeight, Height() - ScrollHeight());
    return offsetHeight;
}

int TerminalVerticalScroll::ItemsPerCell() {
    int viewItems = listView->ViewItems();
    int totalItems = listView->TotalItems();
    return (int)round((double)totalItems / viewItems);
}
