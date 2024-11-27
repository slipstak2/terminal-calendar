#include "TerminalControlsConfig.h"

TerminalControlsConfig& TControlsConfig() {
    static TerminalControlsConfig config;
    return config;
}
