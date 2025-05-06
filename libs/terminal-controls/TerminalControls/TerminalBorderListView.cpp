#include "TerminalBorderListView.h"
#include "TerminalListView.h"
#include "TerminalGroupBox.h"
#include "TerminalLabel.h"
#include "TerminalVerticalScrollBar.h"


TerminalBorderListView::TerminalBorderListView(const Utf8String& title, TerminalCoord position, TerminalSize size)
    : TerminalBorderControl(title, position, size)
{
    listView = TerminalListView::Create(
        TerminalCoord{ .row = 0, .col = 0 },
        TerminalSize{ .height = Height() - 2, .width = Width() - 2 });

    verticalScrollbar = TerminalVerticalScrollBar::Create(
        listView, 
        TerminalCoord{.row = 1, .col = Width() - 1},
        TerminalSize{.height = Height() - 2, .width = 1});

    AddControl(listView);
    AddControlOnBorder(verticalScrollbar);

    GetTitle()->SetFocusable(false);
    GetTitle()->AddClickCallback([this](const MouseContext& ctx) {
        if (listView->GetSelectedRow() == -1) {
            return false;
        }
        if (!listView->IsSelectedRowInView()) {
            listView->NavigateOnSelectedRow();
        }
        else {
            listView->SetSelectedRow(-1);
        }
        return true;
        });
}

void TerminalBorderListView::AddItem(const std::string& value) {
    listView->AddItem(value);
}

bool TerminalBorderListView::RemoveLastItem() {
    return listView->RemoveLastItem();
}
