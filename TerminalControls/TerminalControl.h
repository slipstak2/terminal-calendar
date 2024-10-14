#pragma once

#include "utils/common.h"
#include "TerminalRectangle.h"
#include "TerminalCell.h"
#include <functional>

#define DECLARE_KIND(base, kind)                                    \
    static const TerminalControl::Kind KIND = kind;                 \
    TerminalControl::Kind GetKind() const override { return KIND; } \
    bool IsKindOf(TerminalControl::Kind _kind) const override { return _kind != KIND ? base::IsKindOf(_kind) : true; }

using ClickCallback = std::function<bool()>;

class TerminalControl : public TerminalRectangle {
    friend class TerminalCanvas;
public:
    enum class Kind {
        CONTROL,
        LABEL,
        BUTTON,
        WINDOW
    };
    static const TerminalControl::Kind KIND = Kind::CONTROL;
    virtual Kind GetKind() const { return KIND; };
    virtual bool IsKindOf(Kind kind) const { return kind == KIND; }
public:
    TerminalControl(TerminalCoord position);
    TerminalControl(TerminalCoord position, TerminalSize size);
    TerminalControl(TerminalCoord position, TerminalSize size, TextStyle textStyle);
    virtual void AddControl(TerminalControlPtr control);

    void SetSize(TerminalSize size);

    void SetParentWindow(TerminalWindow* parentWindow);

    template<class ...Args>
    TerminalCell CreateCell(Args... args) {
        TerminalCell cell(std::forward<Args>(args)...);
        cell.SetParent(this);
        cell.SetTextStyle(textStyle);
        return cell;
    }
    const TerminalCell& Get(short row, short col);

    template<class T>
    const T* As() const {
        return this->IsKindOf(T::KIND) ? static_cast<const T*>(this) : nullptr;
    }

    void AddClickCallback(ClickCallback clkCallback) {
        clickCallbacks.push_back(std::move(clkCallback));
    }

    bool ApplyMouseLeftClick() {
        bool isApply = false;
        for (auto clickCallback : clickCallbacks) {
            isApply |= clickCallback();
        }
        return isApply;
    }
    TerminalWindow* GetParentWindow() {
        return parentWindow;
    }
public:
    void Flush();
protected:
    void FlushControls();
    virtual void FlushSelf() = 0;

protected:
    TerminalControlPtr parent = nullptr;
    TerminalWindow* parentWindow = nullptr;
    std::vector<TerminalControlPtr> controls;
    std::vector<std::vector<TerminalCell>> data;

    std::vector<ClickCallback> clickCallbacks;

    TextStyle textStyle = TextStyle::Default;
};