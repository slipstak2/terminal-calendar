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

#include "TerminalCalendarApplication.h"

TerminalCalendarApplication::TerminalCalendarApplication()
    : TerminalApplication() {

    TerminalSize size = canvas->Size();
    short rows = size.height, cols = size.width;

    auto backgroundWindow = TerminalWindow::Create("", TerminalCoord{ .row = 0, .col = 0 }, TerminalSize{ .height = 32, .width = 72 });
    AddWindow(backgroundWindow);

    int month = 0;
    for (short row = 0; row < 4; ++row) {
        for (short col = 0; col < 3; ++col) {
            auto monthLabel = TerminalMonthSwitcher::Create(month++, TerminalCoord{
                .row = row * TerminalMonthSwitcher::DefaultHeight(), 
                .col = col * TerminalMonthSwitcher::DefaultWidth()
                });
            backgroundWindow->AddControlOnBorder(monthLabel);
        }
    }
}