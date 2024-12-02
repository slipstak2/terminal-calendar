#include "pch.h"
#include <Windows.h>

#include "TerminalRadioButton.h"
#include "TerminalGroupBox.h"

class TerminalRadioButtonTests : public testing::Test
{
protected:
    TerminalRadioButtonTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~TerminalRadioButtonTests() override { }
};

TEST_F(TerminalRadioButtonTests, Empty) {
    auto checkBox = TerminalRadioButton::Create("", TerminalCoord());
    EXPECT_EQ(checkBox->GetKind(), TerminalControl::Kind::RADIO_BUTTON);
    EXPECT_TRUE(checkBox->IsKindOf(TerminalControl::Kind::COMPOSITE_CONTROL));
}

TEST_F(TerminalRadioButtonTests, CheckNoParent) {
    auto radioButton = TerminalRadioButton::Create("label", TerminalCoord());
    EXPECT_TRUE(radioButton->SetSelected(true));
    EXPECT_TRUE(radioButton->GetSelected());
    EXPECT_FALSE(radioButton->SetSelected(true));

    EXPECT_TRUE(radioButton->SetSelected(false));
    EXPECT_FALSE(radioButton->GetSelected());
}

TEST_F(TerminalRadioButtonTests, CheckSeveralRadioButton) {
    auto groupBox = TerminalGroupBox::Create("title", TerminalCoord(), TerminalSize(100, 100));
    auto rb01 = TerminalRadioButton::Create("label01", TerminalCoord());
    auto rb02 = TerminalRadioButton::Create("label02", TerminalCoord());
    auto rb03 = TerminalRadioButton::Create("label03", TerminalCoord());
   
    std::vector<bool> changedLog;
    auto changeCallback = [rb01, &changedLog](TerminalRadioButton* sender, bool isSelected) {
        if (sender == rb01.get()) {
            changedLog.push_back(isSelected);
        }
        };
    rb01->AddOnChangedCallback(changeCallback);
    rb02->AddOnChangedCallback(changeCallback);
    rb03->AddOnChangedCallback(changeCallback);

    groupBox->AddControl(rb01);
    groupBox->AddControl(rb02);
    groupBox->AddControl(rb03);

    EXPECT_TRUE(rb01->SetSelected(true));
    EXPECT_FALSE(rb02->GetSelected());
    EXPECT_FALSE(rb03->GetSelected());

    EXPECT_TRUE(rb03->SetSelected(true));
    EXPECT_FALSE(rb01->GetSelected());
    EXPECT_FALSE(rb02->GetSelected());

    std::vector<bool> expectedChangeLog{ true, false };
    EXPECT_EQ(changedLog, expectedChangeLog);
}