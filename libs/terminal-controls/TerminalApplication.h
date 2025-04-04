#pragma once

#include "common.h"
#include "TerminalCanvas.h"
#include <Windows.h>
#include <optional>

#include "TimeProfiler.h"


class TerminalApplication {
public:
    virtual ~TerminalApplication() = default;
    TerminalApplication();
    TerminalSize GetTerminalConsoleSize() const;
    void SetTerminalConsoleSize(short rows, short cols);

    void OnKeyEvent(const KEY_EVENT_RECORD& keyEvent);
    void OnKeyPressUpOrDown(bool isUp);
    
    void OnMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent);
    void OnMouseLeftClick(TerminalCoord absPosition, bool isCtrl, bool isFromDoubleClick = false);
    void OnMouseDoubleClick(TerminalCoord absPosition, bool isCtrl);
    void OnMouseUp(TerminalCoord absPosition);
    void OnMouseMoved(TerminalCoord absPosition);
    void OnMouseWheeled(short value);

    void OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent);
    void OnWindowResize(short rows, short cols);

    void Run();

    void AddWindow(TerminalWindowPtr window);

    void FrameRender(bool isFullRender = false);

protected:
    TerminalBorderListViewPtr dbgListView;

protected:
    bool TryMouseOver(TerminalControl* curUnderMouseControl);
    bool TryMouseOut(TerminalControl* curUnderMouseControl);

    TerminalControl* prvUnderMouseControl = nullptr;
protected:
    bool TryDraggingStart(TerminalControl* control, TerminalCoord absPosition);
    bool TryDragging(TerminalCoord absPosition);
    bool TryDraggingStop();
    TerminalCoord draggingBasePoint;
    TerminalControl* draggingControl;

protected:
    void SetFocusControl(TerminalControl* clickControl, TerminalWindow* clickWindow);

public:
    HWND windowHandle = nullptr; // TODO: move to protected;
protected:
    HANDLE inputHandle;
    HANDLE outputHandle;
    TerminalCanvasPtr canvas;
    TerminalRootControlPtr rootControl;
    TerminalControl* focusControl = nullptr;
};

HWND GetHwnd(const std::string& suffixWindowName);