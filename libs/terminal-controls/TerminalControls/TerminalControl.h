#pragma once

#include "common.h"
#include "TerminalGeometry/TerminalRectangle.h"
#include "TerminalCell.h"
#include "TextFormat/FormatSettings.h"
#include <functional>
#include "Contexts/MouseContext.h"
#include "Contexts/KeyContext.h"

#define DECLARE_KIND(base, kind)                                    \
    static const TerminalControl::Kind KIND = kind;                 \
    TerminalControl::Kind GetKind() const override { return KIND; } \
    bool IsKindOf(TerminalControl::Kind _kind) const override { return _kind != KIND ? base::IsKindOf(_kind) : true; }

using FilterControlCallback = std::function<bool(TerminalControlPtr)>;

using ClickCallback = std::function<bool(const MouseContext& ctx)>;
using ClickCallbackWithPosition = std::function<bool(TerminalCoord, TerminalCoord)>;
using MouseWheelCallback = std::function<bool(short)>;
using MouseOverCallback = std::function<bool()>;
using MouseOutCallback = std::function<bool()>;

using KeyPressCallback = std::function<bool(const KeyContext& kctx)>;
using KeyPressUpOrDownCallback = std::function<bool(int key)>;

class SelectionLayer;

class TerminalControl : public TerminalRectangle {
    friend class TerminalCanvas;
public:
    enum class Kind {
        CONTROL,
        COMPOSITE_CONTROL,
        LABEL_BASE,
        LABEL,
        LABEL_FIXED_WIDTH,
        LABEL_DATA_PROVIDER,
        LABEL_SWITCHER,
        BUTTON,
        RADIO_BUTTON,
        CHECK_BOX,
        LIST_VIEW,
        BORDER_LIST_VIEW,
        GROUP_BOX,
        BORDER_CONTROL,
        VERTICAL_SCROLL,
        VERTICAL_SCROLL_BAR,
        MONTH_BOX,
        GRID,
        GRID_CELL,
        MONTH_GRID,
        WINDOW
    };
    static const TerminalControl::Kind KIND = Kind::CONTROL;
    virtual Kind GetKind() const { return KIND; };
    virtual bool IsKindOf(Kind kind) const { return kind == KIND; }
public:
    virtual ~TerminalControl() = default;
    TerminalControl(TerminalCoord position);
    TerminalControl(TerminalCoord position, TerminalSize size);
    TerminalControl(TerminalCoord position, TerminalSize size, FormatSettings formatSettings);
    virtual void AddControl(TerminalControlPtr control);
    size_t RemoveControls(std::vector<TerminalControlPtr>& controls, const FilterControlCallback& filterCallback);

    void SetSize(TerminalSize newSize);
    void Resize(short rows, short cols);
    void SetPosition(TerminalCoord newPosition);

    void SetParentWindow(TerminalWindow* parentWindow);

    template<class ...Args>
    TerminalCell CreateCell(Args... args) {
        TerminalCell cell(std::forward<Args>(args)...);
        cell.SetParent(this);
        return cell;
    }
    template<class ...Args>
    TerminalCell CreateBackgroundCell(Args... args) {
        TerminalCell cell = CreateCell(args...);
        cell.SetFormatSettings(&FormatSettings::Default);
        return cell;
    }
    const TerminalCell& Get(short row, short col);

    template<class T>
    const T* As() const {
        return this->IsKindOf(T::KIND) ? static_cast<const T*>(this) : nullptr;
    }

    template<class T>
    T* As() {
        return this->IsKindOf(T::KIND) ? static_cast<T*>(this) : nullptr;
    }

    void AddClickCallback(ClickCallback clickCallback) {
        clickCallbacks.push_back(std::move(clickCallback));
    }

    void AddClickCallbackWithPosition(ClickCallbackWithPosition clickCallback) {
        clickCallbacksWithPosition.push_back(std::move(clickCallback));
    }

    void AddMouseWheelCallback(MouseWheelCallback mouseWheelCallback) {
        mouseWheelCallbacks.push_back(mouseWheelCallback);
    }

    void AddMouseOverCallback(MouseOverCallback mouseOverCallback) {
        mouseOverCallbacks.push_back(mouseOverCallback);
    }

    void AddMouseOutCallback(MouseOutCallback mouseOutCallback) {
        mouseOutCallbacks.push_back(mouseOutCallback);
    }

    void AddKeyPressUpOrDownCallbacks(KeyPressUpOrDownCallback keyPressUpOrDownCallback) {
        keyPressUpOrDownCallbacks.push_back(keyPressUpOrDownCallback);
    }

    TerminalCoord GetRelativePosition(TerminalCoord absPosition);

    bool ApplyMouseLeftClick(const MouseContext& ctx, TerminalCoord absPosition) {
        bool isApply = false;
        for (auto& clickCallback : clickCallbacks) {
            isApply |= clickCallback(ctx);
        }
        if (!clickCallbacksWithPosition.empty()) {
            TerminalCoord relPosition = GetRelativePosition(absPosition);
            for (auto& clickCallback : clickCallbacksWithPosition) {
                isApply |= clickCallback(relPosition, absPosition);
            }
        }
        return isApply;
    }
    bool ApplyMouseWheeled(short value) {
        if (!mouseWheelCallbacks.empty()) {
            bool isApply = false;
            for (auto& mouseWheelCallback : mouseWheelCallbacks) {
                isApply |= mouseWheelCallback(value);
            }
            return isApply;
        } else if (parent) {
            return parent->ApplyMouseWheeled(value);
        }
        return false;
    }

    bool ApplyMouseOver() {
        bool isApply = false;
        for (auto& mouseOverCallback : mouseOverCallbacks) {
            isApply |= mouseOverCallback();
        }
        return isApply;
    }

    bool ApplyMouseOut() {
        bool isApply = false;
        for (auto& mouseOutCallback : mouseOutCallbacks) {
            isApply |= mouseOutCallback();
        }
        return isApply;
    }

    bool ApplyKeyPress(KeyContext& ctx);
    bool ApplyKeyPressUpOrDown(bool isUp);

    TerminalWindow* GetParentWindow() {
        return parentWindow;
    }

    TerminalControl* GetParent() {
        return parent;
    }

    const FormatSettings& GetFormatSettings() {
        return formatSettings;
    }

    void SetFormatSettings(FormatSettings newFormatSettings) {
        formatSettings = newFormatSettings;
    }

    void SetTextStyle(TextStyle textStyle) {
        formatSettings.textStyle = textStyle;
    }

    void SetBackgroundColor(BackgroundColor backgroundColor) {
        formatSettings.backgoundRBG = nullptr;
        formatSettings.backgroundColor = backgroundColor;
    }

    void SetBackgroundColor(RGB backgroundColor) {
        formatSettings.backgoundRBG = std::make_shared<RGB>(backgroundColor);
    }

    void SetFontColor(FontColor fontColor) {
        formatSettings.fontRBG = nullptr;
        formatSettings.fontColor = fontColor;
    }
    
    void SetFontColor(RGB fontColor) {
        formatSettings.fontRBG = std::make_shared<RGB>(fontColor);
    }

    void SetVisible(bool isVisible) {
        this->isVisible = isVisible;
    }

    bool IsVisible() const {
        return isVisible;
    }

    bool IsFocusable() {
        return isFocusable;
    }

    void SetFocusable(bool isFocusable) {
        this->isFocusable = isFocusable;
    }
 
    virtual bool IsDraggable() {
        return false;
    }

    virtual bool TryDraggingStart(TerminalCoord absPosition) {
        return false;
    }

    virtual bool TryDragging(TerminalCoord delta) {
        return false;
    }
    
    virtual bool TryDraggingStop() {
        return false;
    }

    bool AllowUseDoubleClickAsSingleClick() {
        return allowUseDoubleClickAsSingleClick;
    }

    virtual void SetSelectionLayer(SelectionLayer* selectionLayer);

    SelectionLayer* GetSelectionLayer();

    std::vector<TerminalControlPtr>& GetControls();

public:
    void Flush();
protected:
    void FlushControls();
    virtual void FlushSelf() = 0;

protected:
    TerminalControl* parent = nullptr;
    TerminalWindow* parentWindow = nullptr;
    SelectionLayer* selectionLayer = nullptr;

    std::vector<TerminalControlPtr> controls;
    std::vector<std::vector<TerminalCell>> data;
    TerminalCell backgroundCell = CreateBackgroundCell('~');

    std::vector<ClickCallback> clickCallbacks;
    std::vector<ClickCallbackWithPosition> clickCallbacksWithPosition;
    std::vector<MouseWheelCallback> mouseWheelCallbacks;
    // https://learn.javascript.ru/mousemove-mouseover-mouseout-mouseenter-mouseleave
    std::vector<MouseOverCallback> mouseOverCallbacks;
    std::vector<MouseOutCallback> mouseOutCallbacks;

    std::vector<KeyPressCallback> keyPressCallbacks;
    std::vector<KeyPressUpOrDownCallback> keyPressUpOrDownCallbacks;

    FormatSettings formatSettings = FormatSettings::Default;
    bool isVisible = true;
    bool isFocusable = true;

    bool allowUseDoubleClickAsSingleClick = true;

public:
    int tag; // for debug don't erase
};