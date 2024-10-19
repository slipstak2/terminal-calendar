#pragma once
#include "TerminalCompositeControl.h"

class TerminalLabelSwitcher : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::LABEL_SWITCHER)

    DECLARE_CREATE(TerminalLabelSwitcher)

    TerminalLabelSwitcher(ListDataProviderPtr dataProvider, TerminalCoord position);

protected:
    void CheckState();
protected:
    ListDataProviderPtr dataProvider;

    TerminalButtonPtr btnLeft;
    TerminalLabelDataProviderPtr  label;
    TerminalButtonPtr btnRight;
};