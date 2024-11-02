#pragma once

#include "TerminalBorderControl.h"

class TerminalWindow : public TerminalBorderControl {
    //friend class TerminalCanvas;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::WINDOW)

    DECLARE_CREATE(TerminalWindow)

    TerminalWindow(const Utf8String& title, TerminalCoord position, TerminalSize size)
        :TerminalBorderControl(title, position, size)
    {
        SetParentWindow(this);
        SetBorderFormat(BorderFormat::Window);
    }
    void AddControl(TerminalControlPtr control) override;
};
