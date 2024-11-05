#include "TerminalVerticalScroll.h"
#include "TerminalListView.h"


TerminalVerticalScroll::TerminalVerticalScroll(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
    , listView(listView)
{
    backgroundCell = CreateBackgroundCell(' ');
}

void TerminalVerticalScroll::FlushSelf() {
    int viewItems = listView->Height();
    int totalItems = listView->TotalItems();

    int totalScroll = Height();
    int scrollH = (int)round((double)viewItems * totalScroll / totalItems);

    int offsetH = (int)round((double)viewItems * listView->viewOffset / totalItems);

    for (int row = 0; row < offsetH; ++row) {
        data[row][0] = backgroundCell;
    }
    for (int row = offsetH; row < std::min(offsetH + scrollH, (int)Height()); ++row) {
        data[row][0] = CreateCell("█");
    }

    for (int row = offsetH + scrollH; row < Height(); ++row) {
        data[row][0] = backgroundCell;
    }
}