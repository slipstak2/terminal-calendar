#define _CRT_SECURE_NO_WARNINGS

#include "common.h"
#include "TerminalApplication.h"
#include "TerminalCanvas.h"
#include "TerminalLabel.h"
#include "TerminalLabelFixedWidth.h"
#include "TerminalButton.h"
#include "TerminalWindow.h"
#include "TerminalRootControl.h"
#include "TerminalLabelSwitcher.h"
#include "DataProviders/ListDataProvider.h"
#include "TerminalListView.h"
#include "TerminalRadioButton.h"
#include "TerminalBorderListView.h"
#include "TerminalCheckBox.h"
#include "TerminalTextBox.h"
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
void TerminalCalendarApplication::UpdateSelectedDayCount() {
    std::string text = std::to_string(selectedDays);
    while (text.size() < 3) {
        text = " " + text;
    }
    selectedDaysCounterLabel->SetText(text);
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

    selectedDaysCounterLabel= TerminalLabelFixedWidth::Create("  0", TerminalCoord{ .row = 0, .col = 3 });
    selectedDaysCounterLabel->SetBackgroundColor(RGB::Brightcyan);
    selectedDaysCounterLabel->SetFontColor(FontColor::Black);
    backgroundWindow->AddControlOnBorder(selectedDaysCounterLabel);

    // Без закругление вверху
    {
        backgroundWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 0, .col = 0 }));
        backgroundWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 1, .col = 0 }));

        backgroundWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 0, .col = backgroundWindow->Width() - 1 }));
        backgroundWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 1, .col = backgroundWindow->Width() - 1 }));
    }

    {
        backgroundWindow->AddControl(TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 7 }, TerminalSize{ .height = 1, .width = 16 }));
        backgroundWindow->AddControl(TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 27 }, TerminalSize{ .height = 1, .width = 16 }));
        backgroundWindow->AddControl(TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 45 }, TerminalSize{ .height = 1, .width = 10 }));
    }

    auto fillMonthsData = [this, backgroundWindow](int year) {
        size_t removeControls = backgroundWindow->RemoveControlsOnBorder([](TerminalControlPtr control) {
            return control->As<TerminalMonthBox>() != nullptr;
        });
        daysSelectionLayer.Clear();
        selectedDays = 0;
        UpdateSelectedDayCount();
        assert(removeControls == 0 || removeControls == 12);

        short offset_row = 2;
        int month = 0;
        for (short row = 0; row < 4; ++row) {
            for (short col = 0; col < 3; ++col) {
                auto monthBox = TerminalMonthBox::Create(year, month++, TerminalCoord{
                    .row = offset_row + row * TerminalMonthBox::DefaultHeight(),
                    .col = col * TerminalMonthBox::DefaultWidth()
                    });
                monthBox->SetSelectionLayer(&daysSelectionLayer);
                monthBox->AddOnCellSelectedCallback([this](TerminalGridCell* sender) {
                    selectedDays += (sender->IsSelected() ? 1 : -1);
                    UpdateSelectedDayCount();
                    });
                backgroundWindow->AddControlOnBorder(monthBox);
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