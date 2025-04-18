#include "TerminalGrid.h"

class TerminalMonthGrid : public TerminalGrid {
public:
    DECLARE_KIND(TerminalGrid, TerminalControl::Kind::MONTH_GRID)

    DECLARE_CREATE(TerminalMonthGrid)

    TerminalMonthGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position);

    void SetSelectionLayer(SelectionLayer* selectionLayer) override;
};