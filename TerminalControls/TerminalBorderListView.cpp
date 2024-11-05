#include "TerminalBorderListView.h"
#include "TerminalListView.h"
#include "TerminalGroupBox.h"
#include "TerminalVerticalScrollBar.h"


TerminalBorderListView::TerminalBorderListView(const Utf8String& title, TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
{
    groupBox = TerminalGroupBox::Create(
        title,
        TerminalCoord{ .row = 0, .col = 0 },
        TerminalSize{ .height = Height(), .width = Width()});
    AddControl(groupBox);

    listView = TerminalListView::Create(
        TerminalCoord{ .row = 0, .col = 0 },
        TerminalSize{ .height = groupBox->Height() - 2, .width = groupBox->Width() - 2 });

    verticalScrollbar = TerminalVerticalScrollBar::Create(
        listView, 
        TerminalCoord{.row = 1, .col = groupBox->Width() - 1},
        TerminalSize{.height = groupBox->Height() - 2, .width = 1});
    //verticalScrollbar->SetVisible(false);

    groupBox->AddControl(listView);
    groupBox->AddControlOnBorder(verticalScrollbar);
}

void TerminalBorderListView::SetBorderColor(FontColor borderColor) {
    groupBox->SetBorderColor(borderColor);
}

void TerminalBorderListView::SetTitleColor(FontColor titleColor) {
    groupBox->SetTitleColor(titleColor);
}

void TerminalBorderListView::AddItem(const std::string& value) {
    listView->AddItem(value);
}

void TerminalBorderListView::SetBorderVisible(bool isVisible) {
    groupBox->SetBorderVisible(isVisible);
}
