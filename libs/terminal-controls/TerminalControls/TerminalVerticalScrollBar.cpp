#include "TerminalVerticalScrollBar.h"
#include "TerminalButton.h"
#include "TerminalVerticalScroll.h"
#include "Interfaces/VerticalScrollableControl.h"


const Utf8String TerminalVerticalScrollBar::UpActive        = "▲";
const Utf8String TerminalVerticalScrollBar::UpInactive      = "△";
const Utf8String TerminalVerticalScrollBar::DownActive      = "▼";
const Utf8String TerminalVerticalScrollBar::DownInactive    = "▽";

TerminalVerticalScrollBar::TerminalVerticalScrollBar(VerticalScrollableControlPtr listView, TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
    , listView(listView)
{
    btnUp = TerminalButton::Create(UpActive, TerminalCoord{.row = 0, .col = 0});
    btnUp->AddClickCallback([listView](const MouseContext& ctx) {
        return listView->ChangeViewOffset(-1);
    });
    AddControl(btnUp);

    verticalScroll = TerminalVerticalScroll::Create(
        listView,
        TerminalCoord{ .row = 1, .col = 0 }, 
        TerminalSize{.height = size.height - 2, .width = size.width});
    verticalScroll->SetFocusable(false); // move focus on vertical scroolbar(may be)

    verticalScroll->AddClickCallbackWithPosition([this](TerminalCoord relPosition, TerminalCoord absPosition) {
        assert(relPosition.col == 0);
        if(relPosition.row < verticalScroll->OffsetHeight()) {
            this->listView->ChangeViewOffset(-(this->listView->ViewItems() - 1));
        } else if (relPosition.row >= verticalScroll->OffsetHeight() + verticalScroll->ScrollHeight()) {
            this->listView->ChangeViewOffset(this->listView->ViewItems() - 1);
        }
        else {
            return false; // click on scroll
        }
        return true;
        });
    AddControl(verticalScroll);

    btnDown = TerminalButton::Create(DownActive, TerminalCoord{ .row = Height() - 1, .col = 0});
    btnDown->AddClickCallback([listView](const MouseContext& ctx) {
        return listView->ChangeViewOffset(1);
    });
    AddControl(btnDown);
    CheckState();
}

void TerminalVerticalScrollBar::CheckVisible() {
    SetVisible(listView->NeedScroll());
}

void TerminalVerticalScrollBar::CheckState() {
    btnUp->SetText(listView->CanScrollUp() ? UpActive : UpInactive);
    btnDown->SetText(listView->CanScrollDown() ? DownActive : DownInactive);
}