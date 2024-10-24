#pragma once
#include "TerminalCompositeControl.h"

class TerminalListView : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);
    void AddItem(std::string value);

    void FlushSelf() override;
protected:
    std::vector<TerminalLabelPtr> items;

};