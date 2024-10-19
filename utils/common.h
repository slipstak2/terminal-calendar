#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>

extern short ONE;

#define DECLARE_CREATE(T)                                         \
    template<class... Args>                                       \
    static std::shared_ptr<T> Create(Args... args) {              \
        return std::make_shared<T>(std::forward<Args>(args)...);  \
    }

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

class TerminalLabelDataProvider;
using TerminalLabelDataProviderPtr = std::shared_ptr<TerminalLabelDataProvider>;

class TerminalLabelSwitcher;
using TerminalLabelSwitcherPtr = std::shared_ptr<TerminalLabelSwitcher>;

class TerminalButton;
using TerminalButtonPtr = std::shared_ptr<TerminalButton>;

class ListDataProvider;
using ListDataProviderPtr = std::shared_ptr<ListDataProvider>;
