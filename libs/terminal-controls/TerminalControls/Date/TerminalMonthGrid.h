#include "TerminalGrid.h"

class TerminalMonthGrid : public TerminalGrid {
public:
    DECLARE_KIND(TerminalGrid, TerminalControl::Kind::MONTH_GRID)

    DECLARE_CREATE(TerminalMonthGrid)

    TerminalMonthGrid(const std::vector<Utf8String>& header, DataContainerPtr container, TerminalCoord position);

    void InitData() override;
    void SetSelectionLayer(SelectionLayer* selectionLayer) override;
};