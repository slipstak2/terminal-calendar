#pragma once
#include "TerminalCompositeControl.h"
#include "ListViewDataProvider.h"

class TerminalVerticalScrollBar : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::VERTICAL_SCROLL_BAR)
    DECLARE_CREATE(TerminalVerticalScrollBar)

public:
    TerminalVerticalScrollBar(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size);
    void CheckVisible();

protected:
    TerminalListViewPtr listView;

    TerminalButtonPtr btnUp;
    TerminalVerticalScrollPtr verticalScroll;
    TerminalButtonPtr btnDown;
};