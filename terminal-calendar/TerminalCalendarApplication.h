#pragma once

#include "TerminalApplication.h"
#include "Layers/DaysSelectionLayer.h"

class TerminalCalendarApplication : public TerminalApplication {
public:
    TerminalCalendarApplication();
    void InitCalendarWindow();
    void UpdateSelectedDayCount();
protected:
    TerminalWindowPtr calendarWindow;
    DaysSelectionLayer daysSelectionLayer;

    size_t selectedDays = 0;
    TerminalLabelFixedWidthPtr selectedDaysCounterLabel = nullptr;
};

TerminalCalendarApplication& app();