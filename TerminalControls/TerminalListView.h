#pragma once
#include "TerminalCompositeControl.h"
#include "ListViewDataProvider.h"

class TerminalListView : public TerminalCompositeControl {
    friend class TerminalVerticalScroll;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);
    void AddItem(const std::string& value);
    int TotalItems() const;
    bool ChangeOffset(int delta);
    void FlushSelf() override;

protected:
    int MaxViewOffset();
protected:
    ListViewDataProvider provider;
    int viewOffset = 0;
};