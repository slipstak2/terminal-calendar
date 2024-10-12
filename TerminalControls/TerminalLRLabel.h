#pragma once
#include "TerminalControl.h"

class TerminalLRLabel : public TerminalControl {
public:
    template<class... Args>
    static TerminalLRLabelPtr Create(Args... args) {
        return std::make_shared<TerminalLRLabel>(std::forward<Args>(args)...);
    }
    TerminalLRLabel(ListDataProviderPtr dataProvider, TerminalCoord position);
    void FlushSelf() override {}
protected:
    TerminalButtonPtr btnLeft;
    TerminalLabelPtr  label;
    TerminalButtonPtr btnRight;
};