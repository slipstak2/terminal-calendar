#pragma once

#include "utils/common.h"
#include "TerminalCanvas.h"
#include <Windows.h>


class TerminalApplication {
public:
    TerminalApplication();

    void OnKeyEvent(const KEY_EVENT_RECORD& keyEvent);
    void OnMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent);
    void OnMouseLeftClick(short row, short col, bool isCtrl);
    void OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent);
    void Run();

    void AddWindow(TerminalWindowPtr window);

    void FullRender();

protected:
    TerminalBorderListViewPtr dbgListView;

private:
    HANDLE inputHandle;
    HANDLE outputHandle;
    TerminalCanvasPtr canvas;
    TerminalRootControlPtr rootControl;
};