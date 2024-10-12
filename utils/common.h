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

class TerminalRootControl;
using TerminalRootControlPtr = std::shared_ptr<TerminalRootControl>;

class TerminalLabel;
using TerminalLabelPtr = std::shared_ptr<TerminalLabel>;

class TerminalButton;
using TerminalButtonPtr = std::shared_ptr<TerminalButton>;

class ListDataProvider;
using ListDataProviderPtr = std::shared_ptr<ListDataProvider>;
