#pragma once
#include "TerminalControl.h"

class TerminalListView : public TerminalControl {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);

    void FlushSelf() override;

};