#pragma once

#include "TerminalControl.h"

class TerminalTextBox : public TerminalControl { // TODO; TerminalControl + keyinput
public:
    DECLARE_CREATE(TerminalTextBox)
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::TEXT_BOX)

public:
    TerminalTextBox(TerminalCoord position, TerminalSize size);

protected:
    void FlushSelf() override;
    void FlushSelfAllignLeft(const Utf8String& s);

protected:
    void InitRenderText();
protected:
    Utf8String text;
    Utf8String renderText;
};