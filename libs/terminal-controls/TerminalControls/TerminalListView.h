#pragma once
#include "TerminalCompositeControl.h"
#include "DataProviders/ListViewDataProvider.h"

class TerminalListView;
using TerminalListViewChangedItemsCountCallback = std::function<void(const TerminalListView* listView, size_t curItemsCount, size_t prvItemsCount)>;
using TerminalListViewChangedOffsetCallback = std::function<void(const TerminalListView* listView, int curOffset, int prvOffset)>;

class TerminalListView : public TerminalCompositeControl {
    friend class TerminalVerticalScroll;
    friend class TerminalListViewTests;
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);
    void AddItem(const std::string& value);
    bool RemoveLastItem();

    bool NeedScroll();
    bool HasUp();
    bool HasDown();

    int ViewItems() const;
    int TotalItems() const;

    bool SetOffset(int newOffset);
    bool ChangeOffset(int delta);
    int GetOffset() const;

    void FlushSelf() override;

public:
    bool SetSelectedItem(int itemNum);
    int GetSelectedItem();
    bool IsSelectedItemInView();
    bool NavigateOnSelectedItem();
    bool MoveSelectedItem(bool isUp);
protected:
    void UpdateViewSelectedItem();

protected:
    int MaxViewOffset();
    int NormalizeOffset(int offset);
protected:
    ListDynamicDataSetPtr dataSet = ListDynamicDataSet::Create();
    ListViewDataProvider provider;
    int viewOffset = 0;
    int selectedItem = -1;

public:
    void AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback);
    void AddChangeOffsetCallback(TerminalListViewChangedOffsetCallback changeOffsetCallback);
protected:
    void OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount);
    void OnChangeOffset(int curOffset, int prvOffset);

    std::vector<TerminalListViewChangedItemsCountCallback> changeItemsCountCallbacks;
    std::vector<TerminalListViewChangedOffsetCallback> changeOffsetCallbacks;
};