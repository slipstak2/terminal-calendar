#pragma once

#include "TerminalCompositeControl.h"
#include "data-storage.h"


class TerminalGrid : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::GRID)

    DECLARE_CREATE(TerminalGrid)

    TerminalGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position);

protected:
    DataStoragePtr storage;
};
