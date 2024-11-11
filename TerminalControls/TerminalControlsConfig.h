#pragma once

class TerminalControlsConfig {
public:
    bool simpleRender = false;
    bool profileEnable = false;
};

TerminalControlsConfig& TControlsConfig();
