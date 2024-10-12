#include "TerminalRootControl.h"

TerminalRootControl::TerminalRootControl(short rows, short cols)
    : TerminalControl({ .row = 0, .col = 0 }, { .height = rows, .width = cols })
{}

bool TerminalRootControl::MoveToTop(const TerminalControl* window) {
    auto it = std::find_if(controls.begin(), controls.end(), [window](const TerminalControlPtr& control) {
        return control.get() == window;
        }
    );
    if (it == controls.end()) {
        return false;
    }
    size_t idx = it - controls.begin();
    if (idx == 0) { // background window
        return false;
    }
    if (idx == controls.size() - 1) { // already top
        return false;
    }

    while (idx + 1 < controls.size()) {
        swap(controls[idx], controls[idx + 1]);
        idx++;
    }
    return true;
}