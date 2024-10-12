#pragma once

#include "TerminalControl.h"
#include "TerminalWindow.h"

class TerminalRootControl : public TerminalControl {
public:
    template<class... Args>
    static TerminalRootControlPtr Create(Args... args) {
        return std::make_shared<TerminalRootControl>(std::forward<Args>(args)...);
    }
    TerminalRootControl(short rows, short cols);
    bool MoveToTop(const TerminalControl* window);
    void FlushSelf() {}
};