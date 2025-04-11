#pragma once

#include "TerminalApplication.h"
#include "Layers/DaysSelectionLayer.h"

class TerminalCalendarApplication : public TerminalApplication {
public:
    TerminalCalendarApplication();
protected:
    DaysSelectionLayer daysSelectionLayer;
    size_t selectedDays = 0;
};

TerminalCalendarApplication& app();