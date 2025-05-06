#pragma once
#include "TerminalCompositeControl.h"
#include "DataProviders/ListViewDataProvider.h"

class TerminalVerticalScrollBar : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::VERTICAL_SCROLL_BAR)
    DECLARE_CREATE(TerminalVerticalScrollBar)

public:
    TerminalVerticalScrollBar(VerticalScrollableControlPtr control, TerminalCoord position, TerminalSize size);
    void CheckVisible();
    void CheckState();

protected:
    VerticalScrollableControlPtr control;

    TerminalButtonPtr btnUp;
    TerminalVerticalScrollPtr verticalScroll;
    TerminalButtonPtr btnDown;

protected:
    static const Utf8String UpActive;
    static const Utf8String UpInactive;
    static const Utf8String DownActive;
    static const Utf8String DownInactive;
};