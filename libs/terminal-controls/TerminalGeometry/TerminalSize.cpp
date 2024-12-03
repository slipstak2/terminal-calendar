#include "TerminalSize.h"
#include <sstream>

std::string TerminalSize::ToString() const {
    std::stringstream ss;
    ss << '[' << height << " x " << width << ']';
    return ss.str();
}