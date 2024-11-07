#pragma once

#include "TerminalControl.h"

class TerminalVerticalScroll : public TerminalControl {
    friend class TerminalVerticalScrollBar;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::VERTICAL_SCROLL)
    DECLARE_CREATE(TerminalVerticalScroll)

    TerminalVerticalScroll(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size);
    
    bool IsDraggable() override;
    bool TryDraggingStart(TerminalCoord absPosition) override;
    bool TryDragging(TerminalCoord delta) override;

protected:
    int ScrollHeight();
    int OffsetHeight();
    int ItemsPerCell();

public:
    void FlushSelf() override;
protected:
    TerminalListViewPtr listView;
};