#pragma once

#include <functional>

class VerticalScrollableControl;
using TerminalListViewChangedOffsetCallback = std::function<void(const VerticalScrollableControl* vsControl, int curOffset, int prvOffset)>;

class VerticalScrollableControl {
public:
    virtual int ViewItems() const = 0;
    virtual int TotalItems() const = 0;

    bool NeedScroll() const;
    bool HasUp() const; // CanScrollUp ?
    bool HasDown() const; // CanScrollDown?

    bool SetOffset(int newOffset);
    bool ChangeOffset(int delta);
    int GetOffset() const;

    void AddChangeOffsetCallback(TerminalListViewChangedOffsetCallback changeOffsetCallback);

protected:
    int MaxViewOffset() const;
    int NormalizeOffset(int offset) const;
    void OnChangeOffset(int curOffset, int prvOffset);


protected:
    int viewOffset = 0;
    std::vector<TerminalListViewChangedOffsetCallback> changeOffsetCallbacks;
//    int selectedItem = -1;
};