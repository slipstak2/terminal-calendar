#include "pch.h"
#include <Windows.h>

#include "TerminalListView.h"
#include "TerminalVerticalScroll.h"

class TerminalVerticalScrollTests : public testing::Test
{
protected:
    TerminalVerticalScrollTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~TerminalVerticalScrollTests() override { }
};

TEST_F(TerminalVerticalScrollTests, Empty) {
    auto listView = TerminalListView::Create(TerminalCoord(), TerminalSize{ .height = 5, .width = 5 });
    auto verticalScroll = TerminalVerticalScroll::Create(listView, TerminalCoord(), TerminalSize{ .height = 5, .width = 5 });
    EXPECT_EQ(verticalScroll->GetKind(), TerminalControl::Kind::VERTICAL_SCROLL);
    EXPECT_TRUE(listView->IsKindOf(TerminalControl::Kind::CONTROL));
}

TEST_F(TerminalVerticalScrollTests, CheckScrollHeight) {
    const int HEIGHT = 100;
    auto listView = TerminalListView::Create(TerminalCoord(), TerminalSize{ .height = HEIGHT, .width = 5 });
    auto verticalScroll = TerminalVerticalScroll::Create(listView, TerminalCoord(), TerminalSize{ .height = HEIGHT, .width = 5 });
    for (int i = 0; i < 2 * HEIGHT; ++i) {
        listView->AddItem(std::to_string(i));
    }
    EXPECT_EQ(50, verticalScroll->ScrollHeight());
    EXPECT_EQ(50, verticalScroll->OffsetHeight());
}

