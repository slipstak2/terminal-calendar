#include "TerminalControlRoot.h"

TerminalControlRoot::TerminalControlRoot(short rows, short cols)
    : TerminalControl({ .row = 0, .col = 0 }, { .height = rows, .width = cols })
{}

bool TerminalControlRoot::MoveToTop(const TerminalControl* window) {
    for (size_t idx = 0; idx < controls.size(); ++idx) {
        if (controls[idx].get() == window) {
            if (idx == 0) {
                break;
            }
            while (idx + 1 < controls.size()) {
                swap(controls[idx], controls[idx + 1]);
                idx++;
            }
            return true;
        }
    }
    return false;
}