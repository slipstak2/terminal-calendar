#pragma once

#include "TerminalGrid.h"

class TerminalGridEx : public TerminalGrid {
public:
    DECLARE_KIND(TerminalGrid, TerminalControl::Kind::GRID_EX)

    DECLARE_CREATE(TerminalGridEx)

    TerminalGridEx(const std::vector<Utf8String>& header, DataContainerPtr container, TerminalCoord position);

    void InitData() override;
    void UpdateContainer(DataContainerPtr container);
};