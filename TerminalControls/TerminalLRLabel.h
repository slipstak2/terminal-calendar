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
    void CheckState();
protected:
    ListDataProviderPtr dataProvider;

    TerminalButtonPtr btnLeft;
    TerminalLabelPtr  label;
    TerminalButtonPtr btnRight;
};