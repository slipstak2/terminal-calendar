#pragma once
#include "TerminalCompositeControl.h"

class TerminalLRLabel : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::LABEL)

    DECLARE_CREATE(TerminalLRLabel)

    TerminalLRLabel(ListDataProviderPtr dataProvider, TerminalCoord position);

protected:
    void CheckState();
protected:
    ListDataProviderPtr dataProvider;

    TerminalButtonPtr btnLeft;
    TerminalLabelPtr  label;
    TerminalButtonPtr btnRight;
};