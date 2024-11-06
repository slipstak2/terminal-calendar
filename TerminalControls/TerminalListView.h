#pragma once
#include "TerminalCompositeControl.h"
#include "ListViewDataProvider.h"

class TerminalListView;
using TerminalListViewChangedItemsCountCallback = std::function<void(const TerminalListView* listView, int curItemsCount, int prvItemsCount)>;

class TerminalListView : public TerminalCompositeControl {
    friend class TerminalVerticalScroll;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);
    void AddItem(const std::string& value);
    bool RemoveLastItem();

    int TotalItems() const;
    bool ChangeOffset(int delta);
    void FlushSelf() override;

protected:
    int MaxViewOffset();
    int NormalizeOffset(int offset);
protected:
    ListViewDataProvider provider;
    int viewOffset = 0;

public:
    void AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback);
protected:
    void OnChangeItemsCount(int curItemsCount, int prvItemsCount);
    std::vector<TerminalListViewChangedItemsCountCallback> changeItemsCountCallbacks;
};