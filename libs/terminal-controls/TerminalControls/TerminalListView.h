#pragma once
#include "TerminalCompositeControl.h"
#include "Interfaces/VerticalScrollableControl.h"
#include "DataProviders/ListViewDataProvider.h"

class TerminalListView : public TerminalCompositeControl, public VerticalScrollableControl {
    friend class TerminalVerticalScroll;
    friend class TerminalListViewTests;
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);
    void AddItem(const std::string& value);
    bool RemoveLastItem();

    int ViewItems() const override;
    int TotalItems() const override;

    void FlushSelf() override;

protected:
    void UpdateViewSelectedItem();

protected:
    ListDynamicDataSetPtr dataSet = ListDynamicDataSet::Create();
    ListViewDataProvider provider;
};