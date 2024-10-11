#include "WindowsManager.h"

void WindowsManager::AddWindow(TerminalWindowPtr window) {
    windows.push_back(window);
}

const std::vector<TerminalWindowPtr>& WindowsManager::GetWindows() const {
    return windows;
}

bool WindowsManager::MoveToTop(const TerminalWindow* window) {
    for (size_t idx = 0; idx < windows.size(); ++idx) {
        if (windows[idx].get() == window) {
            if (idx == 0) {
                break;
            }
            while (idx + 1 < windows.size()) {
                swap(windows[idx], windows[idx + 1]);
                idx++;
            }
            return true;
        }
    }
    return false;
}