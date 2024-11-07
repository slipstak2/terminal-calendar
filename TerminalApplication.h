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
    void OnMouseLeftClick(TerminalCoord position, bool isCtrl, bool isFromDoubleClick = false);
    void OnMouseDoubleClick(TerminalCoord position, bool isCtrl);
    void OnMouseUp(TerminalCoord position);
    void OnMouseMoved(TerminalCoord position);

    void OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent);
    void Run();

    void AddWindow(TerminalWindowPtr window);

    void FullRender();

protected:
    TerminalBorderListViewPtr dbgListView;

protected:
    bool TryDraggingStart(TerminalControl* control, TerminalCoord position);
    bool TryDragging(TerminalCoord position);
    bool TryDraggingStop();
    TerminalCoord draggingStartPoint;
    TerminalControl* draggingControl;

private:
    HANDLE inputHandle;
    HANDLE outputHandle;
    TerminalCanvasPtr canvas;
    TerminalRootControlPtr rootControl;
};