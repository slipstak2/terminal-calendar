#pragma once

#include <functional>

class VerticalScrollableControl;
using ChangedOffsetCallback = std::function<void(const VerticalScrollableControl* vsControl, int prvOffset)>;
using ChangeSelectedRowCallback = std::function<void(const VerticalScrollableControl* vsControl, int prvSelectedRow)>;
using ChangedItemsCountCallback = std::function<void(const VerticalScrollableControl* vsControl, size_t prvItemsCount)>;


class VerticalScrollableControl {
public:
    virtual int ViewItems() const = 0;
    virtual int TotalItems() const = 0;

    // ---- viewOffset ----
    bool NeedScroll() const;
    bool HasUp() const; // CanScrollUp ?
    bool HasDown() const; // CanScrollDown?

    bool SetOffset(int newOffset);
    bool ChangeOffset(int delta);
    int GetOffset() const;

    void AddChangeOffsetCallback(ChangedOffsetCallback changeOffsetCallback);

    // ---- selectedRow ----
    bool SetSelectedRow(int rowNum);
    int GetSelectedRow();
    bool IsSelectedRowInView();
    bool NavigateOnSelectedRow();
    bool MoveSelectedRow(bool isUp);

    void AddChangeSelectedRowCallback(ChangeSelectedRowCallback changeSelectedRowCallback);

    void AddChangeItemsCountCallback(ChangedItemsCountCallback changeItemsCountCallback);

protected:
    int MaxViewOffset() const;
    int NormalizeOffset(int offset) const;
    void OnChangeOffset(int prvOffset);
    void OnChangeSelectedRow(int prvSelectedRow);
    void OnChangeItemsCount(size_t prvItemsCount);


protected:
    int viewOffset = 0;
    std::vector<ChangedOffsetCallback> changeOffsetCallbacks;

    int selectedRow = -1;
    std::vector<ChangeSelectedRowCallback> changeSelectedRowCallbacks;

    std::vector<ChangedItemsCountCallback> changeItemsCountCallbacks;
};