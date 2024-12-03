#pragma once

#include <string>

struct TerminalSize {
    short height = 0;
    short width = 0;

    std::string ToString() const;
};
