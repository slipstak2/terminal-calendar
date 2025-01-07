#include "common.h"
#include "TerminalApplication.h"
#include "TerminalCanvas.h"
#include "TerminalLabel.h"
#include "TerminalButton.h"
#include "TerminalWindow.h"
#include "TerminalRootControl.h"
#include "TerminalLabelSwitcher.h"
#include "DataProviders/ListDataProvider.h"
#include "TerminalListView.h"
#include "TerminalRadioButton.h"
#include "TerminalBorderListView.h"
#include "TerminalCheckBox.h"
#include "TerminalControlsConfig.h"

#include "DemoTerminalApplication.h"

DemoTerminalApplication::DemoTerminalApplication() 
    : TerminalApplication() {

    TerminalSize size = canvas->Size();
    short rows = size.height, cols = size.width;
   
    auto backgroundWindow = TerminalWindow::Create("", TerminalCoord{ .row = 0, .col = 0 }, TerminalSize{ .height = rows, .width = cols });
    AddWindow(backgroundWindow);

    auto isProfileEnable = TerminalCheckBox::Create("Time profiler ",
        TerminalCoord{ .row = backgroundWindow->Height() - 1, .col = 3 });
    isProfileEnable->AddOnChangedCallback([this](TerminalCheckBox* sender, bool isChecked) {
        TControlsConfig().profileEnable = isChecked;
        });
    isProfileEnable->SetLabelFormatSettings(FormatSettings{ .fontColor = FontColor::Yellow });
    backgroundWindow->AddControlOnBorder(isProfileEnable);

    auto isFullRender = TerminalCheckBox::Create("Full render ", TerminalCoord{ .row = backgroundWindow->Height() - 1, .col = isProfileEnable->ColEnd() + 2 });
    isFullRender->AddOnChangedCallback([this](TerminalCheckBox* sender, bool isChecked) {
        TControlsConfig().isFullRender = isChecked;
        });
    isFullRender->SetLabelFormatSettings(FormatSettings{ .fontColor = FontColor::Brightblue });
    backgroundWindow->AddControlOnBorder(isFullRender);

    auto isSimpleRender = TerminalCheckBox::Create("Simple render ", TerminalCoord{ .row = backgroundWindow->Height() - 1, .col = isFullRender->ColEnd() + 2 });
    isSimpleRender->SetLabelFormatSettings(FormatSettings{ .fontColor = FontColor::Green });
    isSimpleRender->AddOnChangedCallback([this](TerminalCheckBox* sender, bool isChecked) {
        TControlsConfig().isSimpleRender = isChecked;
        FrameRender(true);
        });
    backgroundWindow->AddControlOnBorder(isSimpleRender);


    dbgListView = TerminalBorderListView::Create("Debug info",
        TerminalCoord{ .row = 0, .col = 88 },
        TerminalSize{ .height = 28, .width = 30 });

    dbgListView->SetBorderColor(FontColor::Magenta);
    dbgListView->SetTitleColor(FontColor::Yellow);
    backgroundWindow->AddControl(dbgListView);

    auto borderListView = TerminalBorderListView::Create("BorderListView",
        TerminalCoord{ .row = 0, .col = 68 },
        TerminalSize{ .height = 28, .width = 20 });
    borderListView->SetTitleColor(FontColor::Brightgreen);
    backgroundWindow->AddControl(borderListView);

    TControlsConfig().tp.SetCallback([this](const std::string& message) {
        dbgListView->AddItem(message);
        return true;
        });

    auto IvanWindow = TerminalWindow::Create("Ivan", TerminalCoord{ .row = 3, .col = 4 }, TerminalSize{ .height = 15, .width = 40 });
    IvanWindow->SetBorderColor(FontColor::Yellow);

    auto MonthDataProviderPtr = ListDataProvider::Create(monthsDataSet);
    MonthDataProviderPtr->Next();
    MonthDataProviderPtr->Next();

    auto MonthLabel = TerminalLabelSwitcher::Create(MonthDataProviderPtr, TerminalCoord{ .row = 3, .col = 3 });
    IvanWindow->AddControl(MonthLabel);

    AddWindow(IvanWindow);

    auto IgorWindow = TerminalWindow::Create("Igor", TerminalCoord{ .row = 10, .col = 15 }, TerminalSize{ .height = 13, .width = 45 });
    IgorWindow->SetBorderColor(FontColor::Green);

    std::vector<Utf8String> family{ "Маша", "Верунчик или Берунчик", "Юрик$on", "Митрофан" };
    auto familyDataSet = ListDataSet::Create(family);
    auto familyDataProvider = ListDataProvider::Create(familyDataSet);
    auto LRLabel = TerminalLabelSwitcher::Create(familyDataProvider, TerminalCoord{ .row = 3, .col = 3 });
    LRLabel->SetLabelFormatSettings({ .textStyle = TextStyle::Overline });
    IgorWindow->AddControl(LRLabel);

    AddWindow(IgorWindow);

    auto DanilWindow = TerminalWindow::Create("Danil", TerminalCoord{ .row = 5, .col = 12 }, TerminalSize{ .height = 22, .width = 45 });
    DanilWindow->SetBorderColor(FontColor::Red);

    auto rbBorderCyan = TerminalRadioButton::Create("Border Cyan", TerminalCoord{ .row = 2, .col = 5 });
    DanilWindow->AddControl(rbBorderCyan);

    auto rbBorderBrightcyan = TerminalRadioButton::Create("Border Bright cyan", TerminalCoord{ .row = 3, .col = 5 });
    DanilWindow->AddControl(rbBorderBrightcyan);

    auto cbBorder = TerminalCheckBox::Create("Border visible", TerminalCoord{ .row = 5, .col = 5 });
    cbBorder->SetChecked(true);
    DanilWindow->AddControl(cbBorder);

    auto btnAddNewItem = TerminalButton::Create("+ Add new item", TerminalCoord{ .row = 7, .col = 5 });
    btnAddNewItem->AddClickCallback([borderListView]() {
        static int num = 0;
        borderListView->AddItem("#" + std::to_string(++num) + " message");
        return true;
        });

    auto btnRemoveLastItem = TerminalButton::Create("- Remove last item", TerminalCoord{ .row = 8, .col = 5 });

    btnRemoveLastItem->AddClickCallback([borderListView]() {
        return borderListView->RemoveLastItem();
        });
    DanilWindow->AddControl(btnAddNewItem);
    DanilWindow->AddControl(btnRemoveLastItem);

    for (short i = 0; i < 3; ++i) {
        auto hyperLink = TerminalLabel::Create("Hyper link", TerminalCoord{ .row = 10 + i, .col = 30 });
        hyperLink->AddMouseOverCallback([hyperLink]() {
            hyperLink->SetFormatSettings(FormatSettings{ .fontColor = FontColor::Brightblue, .textStyle = TextStyle::Underline });
            return true;
            });
        hyperLink->AddMouseOutCallback([hyperLink]() {
            hyperLink->SetFormatSettings(FormatSettings::Default);
            return true;
            });
        DanilWindow->AddControl(hyperLink);
    }

    auto btnShowSize = TerminalButton::Create("Show Size   ", TerminalCoord{.row = 11, .col = 5});
    btnShowSize->AddClickCallback([btnShowSize, this]() {
        btnShowSize->SetText(GetTerminalConsoleSize().ToString());
        return true; });

    DanilWindow->AddControl(btnShowSize);

    AddWindow(DanilWindow);

    for (int i = 0; i < 175; ++i) {
        btnAddNewItem->ApplyMouseLeftClick(TerminalCoord());
    }

    auto radioButtonChanged = [rbBorderBrightcyan, rbBorderCyan, borderListView](TerminalRadioButton* sender, bool isSelected) {
        if (!isSelected) {
            return;
        }
        if (sender == rbBorderCyan.get()) {
            borderListView->SetBorderColor(FontColor::Cyan);
        }
        if (sender == rbBorderBrightcyan.get()) {
            borderListView->SetBorderColor(FontColor::Brightcyan);
        }
        };
    rbBorderCyan->AddOnChangedCallback(radioButtonChanged);
    rbBorderBrightcyan->AddOnChangedCallback(radioButtonChanged);

    auto cbBorderChanged = [borderListView](TerminalCheckBox* sender, bool isChecked) {
        borderListView->SetBorderVisible(isChecked);
        };
    cbBorder->AddOnChangedCallback(cbBorderChanged);
}