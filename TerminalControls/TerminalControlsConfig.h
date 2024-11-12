#pragma once

#include "TimeProfiler.h"

class TerminalControlsConfig {
public:
    bool simpleRender = false;
    bool profileEnable = false;
    TimeProfiler tp;
};

TerminalControlsConfig& TControlsConfig();
