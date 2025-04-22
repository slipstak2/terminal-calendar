#pragma once

#include "TerminalLabelBase.h"

class TerminalLabelFixedWidth : public TerminalLabelBase {
public:
    DECLARE_CREATE(TerminalLabelFixedWidth)
    DECLARE_KIND(TerminalLabelBase, TerminalControl::Kind::LABEL_FIXED_WIDTH)

public:
    TerminalLabelFixedWidth(Utf8String text, short width, TerminalCoord position);
    TerminalLabelFixedWidth(TerminalCoord position, TerminalSize size);

    const Utf8String& Get() const override;
    short Length() const override;
    void SetText(const Utf8String& newText);

protected:
    void InitRenderText();
protected:
    Utf8String text;
    Utf8String renderText;
};