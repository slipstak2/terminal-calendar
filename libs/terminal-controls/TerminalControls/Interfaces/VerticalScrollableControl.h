#pragma once

#include <functional>

class VerticalScrollableControl;
using ChangedOffsetCallback = std::function<void(const VerticalScrollableControl* vsControl, int prvOffset)>;
using ChangeSelectedRowCallback = std::function<void(const VerticalScrollableControl* vsControl, int prvSelectedRow)>;


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
    void AddChangeSelectedRowCallback(ChangeSelectedRowCallback changeSelectedRowCallback);

protected:
    int MaxViewOffset() const;
    int NormalizeOffset(int offset) const;
    void OnChangeOffset(int prvOffset);
    void OnChangeSelectedRow(int prvSelectedRow);


protected:
    int viewOffset = 0;
    std::vector<ChangedOffsetCallback> changeOffsetCallbacks;

    int selectedRow = -1;
    std::vector<ChangeSelectedRowCallback> changeSelectedRowCallbacks;
};