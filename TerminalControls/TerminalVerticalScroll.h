#pragma once

#include "TerminalControl.h"

class TerminalVerticalScroll : public TerminalControl {
    friend class TerminalVerticalScrollBar;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::VERTICAL_SCROLL)
    DECLARE_CREATE(TerminalVerticalScroll)

    TerminalVerticalScroll(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size);
protected:
    int ScrollHeight();
    int OffsetHeight();

public:
    void FlushSelf() override;
protected:
    TerminalListViewPtr listView;
};