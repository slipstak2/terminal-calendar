#pragma once

#include "TerminalCompositeControl.h"

class TerminalBorderListView : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::BORDER_LIST_VIEW)
    DECLARE_CREATE(TerminalBorderListView)
public:
    TerminalBorderListView(const Utf8String& title, TerminalCoord position, TerminalSize size);

    void SetBorderColor(FontColor borderColor);
    void SetTitleColor(FontColor titleColor);
    void SetBorderVisible(bool isVisible);
    void AddItem(const std::string& value);
    bool RemoveLastItem();

protected:
    TerminalGroupBoxPtr groupBox;
    TerminalListViewPtr listView;
    TerminalVerticalScrollBarPtr verticalScrollbar;
};