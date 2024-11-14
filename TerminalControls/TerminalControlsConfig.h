#pragma once

#include "TimeProfiler.h"

class TerminalControlsConfig {
public:
    bool isSimpleRender = false;
    bool profileEnable = false;
    bool isFullRender = false;
    TimeProfiler tp;
};

TerminalControlsConfig& TControlsConfig();
