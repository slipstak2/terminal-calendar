#pragma once

#include "TerminalLabel.h"


class TerminalButton : public TerminalLabel {
public:
    DECLARE_KIND(TerminalLabel, TerminalControl::Kind::BUTTON)

    DECLARE_CREATE(TerminalButton)

    TerminalButton(const Utf8String& label, TerminalCoord position) : TerminalLabel(label, position) {}
};