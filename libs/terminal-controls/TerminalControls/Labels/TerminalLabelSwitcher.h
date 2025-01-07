#pragma once
#include "TerminalCompositeControl.h"
#include "DataProviders/ListDataProvider.h"

class TerminalLabelSwitcher : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::LABEL_SWITCHER)

    DECLARE_CREATE(TerminalLabelSwitcher)

    TerminalLabelSwitcher(ListDataProviderPtr dataProvider, TerminalCoord position);

    void SetLabelFormatSettings(const FormatSettings labelFormatSettings);

    void AddChangeCallback(ChangeCallback changeCallback);

protected:
    void CheckState();
protected:
    ListDataProviderPtr dataProvider;

    TerminalButtonPtr btnLeft;
    TerminalLabelDataProviderPtr label;
    TerminalButtonPtr btnRight;
protected:
    static const Utf8String LeftActive;
    static const Utf8String LeftInactive;
    static const Utf8String RightActive;
    static const Utf8String RightInactive;
};