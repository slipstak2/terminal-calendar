#pragma once

class VerticalScrollableControl {
public:
    virtual bool NeedScroll() const = 0;
    virtual bool HasUp() const = 0; // CanScrollUp ?
    virtual bool HasDown() const = 0; // CanScrollDown?

    virtual bool ChangeOffset(int delta) = 0;
    virtual bool SetOffset(int newOffset) = 0;
    virtual int GetOffset() const = 0;

    virtual int ViewItems() const = 0;
    virtual int TotalItems() const = 0;



protected:
    int viewOffset = 0;
    int selectedItem = -1;
};