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
    TerminalControl(TerminalCoord lu, TerminalSize size);
    void AddControl(TerminalControlPtr control);

    template<class ...Args>
    TerminalCell CreateCell(Args... args) {
        TerminalCell cell(std::forward<Args>(args)...);
        cell.SetParent(this);
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

    void ApplyMouseLeftClick() {
        for (auto clickCallback : clickCallbacks) {
            clickCallback();
        }
    }
public:
    void Flush();
protected:
    void FlushControls();
    virtual void FlushSelf() = 0;

protected:
    TerminalControlPtr parent = nullptr;
    std::vector<TerminalControlPtr> controls;
    std::vector<std::vector<TerminalCell>> data;

    std::vector<ClickCallback> clickCallbacks;

};