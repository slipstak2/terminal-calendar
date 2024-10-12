#pragma once
#include "TerminalCompositeControl.h"

class TerminalLRLabel : public TerminalCompositeControl {
public:
    template<class... Args>
    static TerminalLRLabelPtr Create(Args... args) {
        return std::make_shared<TerminalLRLabel>(std::forward<Args>(args)...);
    }
    TerminalLRLabel(ListDataProviderPtr dataProvider, TerminalCoord position);
protected:
    TerminalButtonPtr btnLeft;
    TerminalLabelPtr  label;
    TerminalButtonPtr btnRight;
};