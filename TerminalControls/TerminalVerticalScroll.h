#pragma once

#include "TerminalControl.h"

class TerminalVerticalScroll : public TerminalControl {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::VERTICAL_SCROLL)
    DECLARE_CREATE(TerminalVerticalScroll)

    TerminalVerticalScroll(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size);
    bool NeedScroll();

public:
    void FlushSelf() override;
protected:
    TerminalListViewPtr listView;
};