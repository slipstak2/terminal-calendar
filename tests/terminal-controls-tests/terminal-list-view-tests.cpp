#include "pch.h"
#include <Windows.h>

#include "TerminalListView.h"

class TerminalListViewTests : public testing::Test
{
protected:
    TerminalListViewTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~TerminalListViewTests() override { }
};

TEST_F(TerminalListViewTests, Empty) {
    auto listView = TerminalListView::Create(TerminalCoord(), TerminalSize{.height = 5, .width = 5 });
    EXPECT_EQ(listView->GetKind(), TerminalControl::Kind::LIST_VIEW);
    EXPECT_TRUE(listView->IsKindOf(TerminalControl::Kind::COMPOSITE_CONTROL));
}

TEST_F(TerminalListViewTests, TestKeyUpAndDown) {
    const int HEIGHT = 6;
    auto listView = TerminalListView::Create(TerminalCoord(), TerminalSize{ .height = HEIGHT, .width = 5 });
    for (int i = 0; i <= HEIGHT; ++i) {
        listView->AddItem(std::to_string(i));
    }
    EXPECT_TRUE(listView->NeedScroll());
    listView->SetSelectedRow(listView->TotalItems() - 1);
    EXPECT_EQ(1, listView->GetViewOffset());
    EXPECT_TRUE(listView->CanScrollUp());
    EXPECT_FALSE(listView->CanScrollDown());

    for (int i = 0; i < HEIGHT - 1; ++i) {
        EXPECT_TRUE(listView->MoveSelectedRow(true));
        EXPECT_EQ(1, listView->GetViewOffset());
    }
    EXPECT_TRUE(listView->MoveSelectedRow(true));
    EXPECT_EQ(0, listView->GetViewOffset());
    EXPECT_FALSE(listView->MoveSelectedRow(true));
}