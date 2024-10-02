#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>

class TerminalWindow;
using TerminalWindowPtr = std::shared_ptr<TerminalWindow>;

class TerminalCanvas;
using TerminalCanvasPtr = std::unique_ptr<TerminalCanvas>;

class TerminalControl;
using TerminalControlPtr = std::shared_ptr<TerminalControl>;

class TerminalLabel;
using TerminalLabelPtr = std::shared_ptr<TerminalLabel>;