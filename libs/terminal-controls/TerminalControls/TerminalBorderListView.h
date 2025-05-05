#pragma once

#include "TerminalBorderControl.h"

class TerminalBorderListView : public TerminalBorderControl {
public:
    DECLARE_KIND(TerminalBorderControl, TerminalControl::Kind::BORDER_LIST_VIEW)
    DECLARE_CREATE(TerminalBorderListView)
public:
    TerminalBorderListView(const Utf8String& title, TerminalCoord position, TerminalSize size);

    void AddItem(const std::string& value);
    bool RemoveLastItem();

protected:
    TerminalListViewPtr listView;
    TerminalVerticalScrollBarPtr verticalScrollbar;
};