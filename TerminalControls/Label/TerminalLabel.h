#pragma once
#include "utils/common.h"
#include "utils/utf8string.h"
#include "TerminalLabelBase.h"

class TerminalLabel : public TerminalLabelBase {
public:
    DECLARE_KIND(TerminalLabelBase, TerminalControl::Kind::LABEL)

    DECLARE_CREATE(TerminalLabel)

public:
    TerminalLabel(const Utf8String& label, TerminalCoord position);
    TerminalLabel(TerminalCoord position, TerminalSize size);

    const Utf8String& Get() const override;
    short Length() const override;
    void SetText(const Utf8String& newText);

protected:
    Utf8String text;
};