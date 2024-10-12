#pragma once

#include "TerminalControl.h"
#include "TerminalWindow.h"

class TerminalControlRoot : public TerminalControl {
public:
    template<class... Args>
    static TerminalControlRootPtr Create(Args... args) {
        return std::make_shared<TerminalControlRoot>(std::forward<Args>(args)...);
    }
    TerminalControlRoot(short rows, short cols);
    bool MoveToTop(const TerminalControl* window);
    void FlushSelf() {}
};