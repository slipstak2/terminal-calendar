#include "pch.h"
#include <Windows.h>

#include "TerminalCheckBox.h"

class TerminalCheckBoxTests : public testing::Test
{
protected:
    TerminalCheckBoxTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~TerminalCheckBoxTests() override { }
};

TEST_F(TerminalCheckBoxTests, Empty) {
    auto checkBox = TerminalCheckBox::Create("", TerminalCoord());
    EXPECT_EQ(checkBox->GetKind(), TerminalControl::Kind::CHECK_BOX);
    EXPECT_TRUE(checkBox->IsKindOf(TerminalControl::Kind::COMPOSITE_CONTROL));
}

TEST_F(TerminalCheckBoxTests, CheckSet) {
    auto checkBox = TerminalCheckBox::Create("label", TerminalCoord());
    std::vector<bool> changedLog;
    checkBox->AddOnChangedCallback([checkBox, &changedLog](const MouseContext& ctx, TerminalCheckBox* sender, bool isChecked) {
        EXPECT_EQ(sender, checkBox.get());
        changedLog.push_back(isChecked);
        });

    EXPECT_FALSE(checkBox->GetChecked());
    EXPECT_TRUE(checkBox->SetChecked(true));
    EXPECT_TRUE(checkBox->GetChecked());
    EXPECT_FALSE(checkBox->SetChecked(true));
    EXPECT_TRUE(checkBox->SetChecked(false));
    EXPECT_FALSE(checkBox->GetChecked());
    
    std::vector<bool> expectedChangeLog{ true, false };
    EXPECT_EQ(changedLog, expectedChangeLog);
}