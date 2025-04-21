#pragma once

#include "TerminalApplication.h"
#include "Layers/DaysSelectionLayer.h"

class TerminalCalendarApplication : public TerminalApplication {
public:
    TerminalCalendarApplication();

    void InitCalendarWindow();
    void InitSearchWindow();

    void UpdateSelectedDayCount();
protected:
    TerminalWindowPtr calendarWindow;
    TerminalWindowPtr searchWindow;
    DaysSelectionLayer daysSelectionLayer;

    size_t selectedDays = 0;
    TerminalLabelFixedWidthPtr selectedDaysCounterLabel = nullptr;
};

TerminalCalendarApplication& app();