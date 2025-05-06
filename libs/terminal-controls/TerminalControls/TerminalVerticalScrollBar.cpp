#include "TerminalVerticalScrollBar.h"
#include "TerminalButton.h"
#include "TerminalVerticalScroll.h"
#include "Interfaces/VerticalScrollableControl.h"


const Utf8String TerminalVerticalScrollBar::UpActive        = "▲";
const Utf8String TerminalVerticalScrollBar::UpInactive      = "△";
const Utf8String TerminalVerticalScrollBar::DownActive      = "▼";
const Utf8String TerminalVerticalScrollBar::DownInactive    = "▽";

TerminalVerticalScrollBar::TerminalVerticalScrollBar(VerticalScrollableControlPtr control, TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
    , control(control)
{
    btnUp = TerminalButton::Create(UpActive, TerminalCoord{.row = 0, .col = 0});
    btnUp->AddClickCallback([control](const MouseContext& ctx) {
        return control->ChangeViewOffset(-1);
    });
    AddControl(btnUp);

    verticalScroll = TerminalVerticalScroll::Create(
        control,
        TerminalCoord{ .row = 1, .col = 0 }, 
        TerminalSize{.height = size.height - 2, .width = size.width});
    verticalScroll->SetFocusable(false); // move focus on vertical scroolbar(may be)

    verticalScroll->AddClickCallbackWithPosition([this](TerminalCoord relPosition, TerminalCoord absPosition) {
        assert(relPosition.col == 0);
        if(relPosition.row < verticalScroll->OffsetHeight()) {
            this->control->ChangeViewOffset(-(this->control->ViewItems() - 1));
        } else if (relPosition.row >= verticalScroll->OffsetHeight() + verticalScroll->ScrollHeight()) {
            this->control->ChangeViewOffset(this->control->ViewItems() - 1);
        }
        else {
            return false; // click on scroll
        }
        return true;
        });
    AddControl(verticalScroll);

    btnDown = TerminalButton::Create(DownActive, TerminalCoord{ .row = Height() - 1, .col = 0});
    btnDown->AddClickCallback([control](const MouseContext& ctx) {
        return control->ChangeViewOffset(1);
    });
    AddControl(btnDown);

    CheckState();
    CheckVisible();

    control->AddChangeItemsCountCallback([this](const VerticalScrollableControl* _, size_t prvItemsCount) {
        CheckVisible();
    });

    control->AddChangeOffsetCallback([this](const VerticalScrollableControl* _, int prvOffset) {
        CheckState();
    });
}

void TerminalVerticalScrollBar::CheckVisible() {
    SetVisible(control->NeedScroll());
}

void TerminalVerticalScrollBar::CheckState() {
    btnUp->SetText(control->CanScrollUp() ? UpActive : UpInactive);
    btnDown->SetText(control->CanScrollDown() ? DownActive : DownInactive);
}