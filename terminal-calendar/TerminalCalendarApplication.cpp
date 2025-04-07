#define _CRT_SECURE_NO_WARNINGS

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
#include "Date/TerminalMonthBox.h"
#include "DataProviders/ListDataSetNumSequence.h"

#include "TerminalCalendarApplication.h"

TerminalCalendarApplication& app() {
    static TerminalCalendarApplication app;
    return app;
}

void UpdateData(TerminalCalendarApplication* self, int dh, int dw, TerminalLabelPtr sizeLabel, TerminalLabelPtr tSizeLabel) {
    RECT rect;
    GetWindowRect(self->windowHandle, &rect);
    int W = rect.right - rect.left;
    int H = rect.bottom - rect.top;
    SetWindowPos(self->windowHandle, NULL, 100, 100, W + dw, H + dh, SWP_SHOWWINDOW);

    GetWindowRect(self->windowHandle, &rect);
    W = rect.right - rect.left;
    H = rect.bottom - rect.top;

    char buf[100];
    sprintf(buf, "%dx%d", H, W);
    sizeLabel->SetText(buf);

    TerminalSize tsize = self->GetTerminalConsoleSize();
    sprintf(buf, "%dx%d", tsize.height, tsize.width);
    tSizeLabel->SetText(buf);
}
TerminalCalendarApplication::TerminalCalendarApplication()
    : TerminalApplication() {

    TerminalSize size = canvas->Size();
    short rows = size.height, cols = size.width;

    auto backgroundWindow = TerminalWindow::Create("", TerminalCoord{ .row = 0, .col = 0 }, TerminalSize{ .height = 38, .width = 72 });
    AddWindow(backgroundWindow);

    auto yearsDataProvider = ListDataProvider::Create(yearsDataSet, yearsDataSet->GetPos(2025));
    short year_label_offset = (backgroundWindow->Width() - 8) / 2;
    auto yearsLabel = TerminalLabelSwitcher::Create(yearsDataProvider, TerminalCoord{ .row = 0, .col = year_label_offset });
    yearsLabel->SetLabelFormatSettings({ .fontColor = FontColor::Green });

    backgroundWindow->AddControlOnBorder(yearsLabel);

    auto sizeLabel = TerminalLabel::Create("XXXxYYY", TerminalCoord{ .row = 0, .col = 13 });
    backgroundWindow->AddControlOnBorder(sizeLabel);

    auto tSizeLabel = TerminalLabel::Create("RRxCC", TerminalCoord{ .row = 0, .col = 50 });
    backgroundWindow->AddControlOnBorder(tSizeLabel);

    auto bntDecH = TerminalButton::Create("[-h]", TerminalCoord{ .row = 0, .col = 3 });
    bntDecH->AddClickCallback([this, sizeLabel, tSizeLabel]()  {
        UpdateData(this, -1, 0, sizeLabel, tSizeLabel);
        return true;
        });
    backgroundWindow->AddControlOnBorder(bntDecH);

    auto bntIncH = TerminalButton::Create("[+h]", TerminalCoord{ .row = 0, .col = 7 });
    bntIncH->AddClickCallback([this, sizeLabel, tSizeLabel]() {
        UpdateData(this, +1, 0, sizeLabel, tSizeLabel);
        return true;
        });
    backgroundWindow->AddControlOnBorder(bntIncH);

    auto bntDecW = TerminalButton::Create("[-w]", TerminalCoord{ .row = 0, .col = 22 });
    bntDecW->AddClickCallback([this, sizeLabel, tSizeLabel]() {
        UpdateData(this, 0, -1, sizeLabel, tSizeLabel);
        return true;
        });
    backgroundWindow->AddControlOnBorder(bntDecW);

    auto bntIncW = TerminalButton::Create("[+w]", TerminalCoord{ .row = 0, .col = 26 });
    bntIncW->AddClickCallback([this, sizeLabel, tSizeLabel]() {
        UpdateData(this, 0, +1, sizeLabel, tSizeLabel);
        return true;
        });
    backgroundWindow->AddControlOnBorder(bntIncW);


    auto fillMonthsData = [backgroundWindow](int year) {
        size_t removeControls = backgroundWindow->RemoveControlsOnBorder([](TerminalControlPtr control) {
            return control->As<TerminalMonthBox>() != nullptr;
        });
        assert(removeControls == 0 || removeControls == 12);

        short offset_row = 2;
        int month = 0;
        for (short row = 0; row < 4; ++row) {
            for (short col = 0; col < 3; ++col) {
                auto monthLabel = TerminalMonthBox::Create(year, month++, TerminalCoord{
                    .row = offset_row + row * TerminalMonthBox::DefaultHeight(),
                    .col = col * TerminalMonthBox::DefaultWidth()
                    });
                backgroundWindow->AddControlOnBorder(monthLabel);
            }
        }
     };

    yearsLabel->AddChangeCallback([fillMonthsData](const Utf8String& prev, const Utf8String& current) {
        int year = (current[0].get()[0] - '0') * 1000 + (current[1].get()[0] - '0') * 100 + (current[2].get()[0] - '0') * 10 + (current[3].get()[0] - '0') * 1;
        fillMonthsData(year);
        return true;
        });

    fillMonthsData(2025);

    SetTerminalConsoleSize(40, 100);
}