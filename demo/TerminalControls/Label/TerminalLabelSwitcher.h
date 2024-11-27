#pragma once
#include "TerminalCompositeControl.h"

class TerminalLabelSwitcher : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::LABEL_SWITCHER)

    DECLARE_CREATE(TerminalLabelSwitcher)

    TerminalLabelSwitcher(ListDataProviderPtr dataProvider, TerminalCoord position);

    void SetLabelFormatSettings(const FormatSettings labelFormatSettings);

protected:
    void CheckState();
protected:
    ListDataProviderPtr dataProvider;

    TerminalButtonPtr btnLeft;
    TerminalLabelDataProviderPtr  label;
    TerminalButtonPtr btnRight;
protected:
    static const Utf8String LeftActive;
    static const Utf8String LeftInactive;
    static const Utf8String RightActive;
    static const Utf8String RightInactive;
};