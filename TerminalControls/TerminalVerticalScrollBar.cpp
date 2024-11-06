#include "TerminalVerticalScrollBar.h"
#include "TerminalButton.h"
#include "TerminalVerticalScroll.h"
#include "TerminalListView.h"

TerminalVerticalScrollBar::TerminalVerticalScrollBar(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
    , listView(listView)
{
    btnUp = TerminalButton::Create("▲", TerminalCoord{.row = 0, .col = 0});
    btnUp->AddClickCallback([listView]() {
        return listView->ChangeOffset(-1);
    });
    AddControl(btnUp);

    verticalScroll = TerminalVerticalScroll::Create(
        listView,
        TerminalCoord{ .row = 1, .col = 0 }, 
        TerminalSize{.height = size.height - 2, .width = size.width});
    AddControl(verticalScroll);

    btnDown = TerminalButton::Create("▼", TerminalCoord{ .row = Height() - 1, .col = 0});
    btnDown->AddClickCallback([listView]() {
        return listView->ChangeOffset(1);
    });
    AddControl(btnDown);
}

void TerminalVerticalScrollBar::CheckVisible() {
    SetVisible(verticalScroll->NeedScroll());
}