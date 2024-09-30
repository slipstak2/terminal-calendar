#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <stdint.h>

class TerminalWindow;
using TerminalWindowPtr = std::shared_ptr<TerminalWindow>;

class TerminalControl;
using TerminalControlPtr = std::shared_ptr<TerminalControl>;

class TerminalLabel;
using TerminalLabelPtr = std::shared_ptr<TerminalLabel>;