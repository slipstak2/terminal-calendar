#pragma once
#include "utils/common.h"
#include "TerminalControl.h"

class TerminalLabel : public TerminalControl {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LABEL)

    DECLARE_CREATE(TerminalLabel)

public:
    TerminalLabel(const Utf8String& label, TerminalCoord position);
    TerminalLabel(ListDataProviderPtr dataProvider, TerminalCoord position, TextStyle textStyle);
    const Utf8String& Get() const;
    void SetText(const Utf8String& newText);

    short Size() const;

protected:
    void FlushSelf() override;
protected:
    Utf8String text;
    ListDataProviderPtr dataProvider = nullptr;
};