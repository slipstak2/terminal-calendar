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
    verticalScrollbar->CheckVisible();

    AddControl(listView);
    AddControlOnBorder(verticalScrollbar);

    listView->AddChangeItemsCallback([this](const TerminalListView* listView, size_t curItemsCount, size_t prvItemsCount) {
        verticalScrollbar->CheckVisible();
        });

    listView->AddChangeOffsetCallback([this](const VerticalScrollableControl* listView, int prvOffset) {
        verticalScrollbar->CheckState();
    });

    GetTitle()->SetFocusable(false);
    GetTitle()->AddClickCallback([this](const MouseContext& ctx) {
        if (listView->GetSelectedItem() == -1) {
            return false;
        }
        if (!listView->IsSelectedItemInView()) {
            listView->NavigateOnSelectedItem();
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
