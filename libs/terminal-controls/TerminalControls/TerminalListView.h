#pragma once
#include "TerminalCompositeControl.h"
#include "Interfaces/VerticalScrollableControl.h"
#include "DataProviders/ListViewDataProvider.h"

class TerminalListView;
using TerminalListViewChangedItemsCountCallback = std::function<void(const TerminalListView* listView, size_t curItemsCount, size_t prvItemsCount)>;

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

public:
    bool SetSelectedRow(int rowNum);
    int GetSelectedItem();
    bool IsSelectedItemInView();
    bool NavigateOnSelectedItem();
    bool MoveSelectedItem(bool isUp);
protected:
    void UpdateViewSelectedItem();

protected:
    ListDynamicDataSetPtr dataSet = ListDynamicDataSet::Create();
    ListViewDataProvider provider;

public:
    void AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback);
protected:
    void OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount);

    std::vector<TerminalListViewChangedItemsCountCallback> changeItemsCountCallbacks;
};