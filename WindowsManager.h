#pragma once
#include "utils/common.h"
#include <vector>

class WindowsManager {
public:
    WindowsManager() = default;
    void AddWindow(TerminalWindowPtr window);
    const std::vector<TerminalWindowPtr>& GetWindows() const;
    bool MoveToTop(const TerminalWindow* window);

protected:
    std::vector<TerminalWindowPtr> windows;
};