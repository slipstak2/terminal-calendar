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
#include "Date/TerminalMonthSwitcher.h"
#include "DataProviders/ListDataSetNumSequence.h"

#include "TerminalCalendarApplication.h"

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

    auto fillMonthsData = [backgroundWindow](int year) {
        int removeControls = backgroundWindow->RemoveControlsOnBorder([](TerminalControlPtr control) {
            return control->As<TerminalMonthSwitcher>() != nullptr;
        });
        assert(removeControls == 0 || removeControls == 12);

        short offset_row = 2;
        int month = 0;
        for (short row = 0; row < 4; ++row) {
            for (short col = 0; col < 3; ++col) {
                auto monthLabel = TerminalMonthSwitcher::Create(year, month++, TerminalCoord{
                    .row = offset_row + row * TerminalMonthSwitcher::DefaultHeight(),
                    .col = col * TerminalMonthSwitcher::DefaultWidth()
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