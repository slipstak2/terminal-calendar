#pragma once

#include "utils/common.h"
#include "TerminalCanvas.h"
#include <Windows.h>
#include <optional>


class TerminalApplication {
public:
    TerminalApplication();

    void OnKeyEvent(const KEY_EVENT_RECORD& keyEvent);
    
    void OnMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent);
    void OnMouseLeftClick(TerminalCoord absPosition, bool isCtrl, bool isFromDoubleClick = false);
    void OnMouseDoubleClick(TerminalCoord absPosition, bool isCtrl);
    void OnMouseUp(TerminalCoord absPosition);
    void OnMouseMoved(TerminalCoord absPosition);
    void OnMouseWheeled(short value);

    void OnKeyPressUpOrDown(bool isUp);

    void OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent);
    void Run();

    void AddWindow(TerminalWindowPtr window);

    void FullRender();

protected:
    TerminalBorderListViewPtr dbgListView;

protected:
    bool TryDraggingStart(TerminalControl* control, TerminalCoord absPosition);
    bool TryDragging(TerminalCoord absPosition);
    bool TryDraggingStop();
    TerminalCoord draggingBasePoint;
    TerminalControl* draggingControl;

protected:
    void SetFocusControl(TerminalControl* clickControl, TerminalWindow* clickWindow);

private:
    HANDLE inputHandle;
    HANDLE outputHandle;
    TerminalCanvasPtr canvas;
    TerminalRootControlPtr rootControl;
    TerminalControl* focusControl = nullptr;
};