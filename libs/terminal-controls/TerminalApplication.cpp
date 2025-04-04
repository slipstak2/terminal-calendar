#include "common.h"
#include "TerminalApplication.h"
#include "TerminalCanvas.h"
#include "TerminalLabel.h"
#include "TerminalButton.h"
#include "TerminalWindow.h"
#include "TerminalRootControl.h"
#include "TerminalLabelSwitcher.h"
#include "DataProviders/ListDataProvider.h"
#include "TerminalControlsConfig.h"

void MyErrorExit(const char* s) {
    printf("Fatal: %s\n", s);
    exit(1);
}

void GetWindowSize(HANDLE outputHandle, short& rows, short& cols) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(outputHandle, &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

TerminalSize TerminalApplication::GetTerminalConsoleSize() const {
    TerminalSize size;
    GetWindowSize(outputHandle, size.height, size.width);
    return size;
}

void TerminalApplication::SetTerminalConsoleSize(short rows, short cols) {
    // TODO: calculate window size with screen resolution
    int width = 696; // 738;
    int height = 787; // 763;

    SetWindowPos(windowHandle, NULL, 100, 100, width, height, SWP_SHOWWINDOW);

    // https://stackoverflow.com/questions/25912721/set-console-window-size-on-windows
    // https://stackoverflow.com/questions/68942746/how-do-i-resize-the-console-window-to-a-set-number-of-rows-and-columns
}

TerminalApplication::TerminalApplication()  {
    windowHandle = GetConsoleWindow();
    windowHandle = GetWindow(windowHandle, GW_OWNER);

    inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    if (inputHandle == NULL) {
        MyErrorExit("GetStdHandle");
    }
    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (outputHandle == NULL) {
        MyErrorExit("GetStdHandle");
    }

    DWORD fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(inputHandle, fdwMode)) {
        MyErrorExit("SetConsoleMode");
    }
    ShowConsoleCursor(false);

    short rows, cols;
    GetWindowSize(outputHandle, rows, cols);
    rootControl = TerminalRootControl::Create(rows, cols);
    canvas = TerminalCanvas::Create(outputHandle, rows, cols);
}

void TerminalApplication::AddWindow(TerminalWindowPtr window) {
    rootControl->AddControl(window);
}

void TerminalApplication::FrameRender(bool isFullRender) {
    if (isFullRender) {
        TControlsConfig().isFullRender = true;
    }

    canvas->Render(rootControl);

    if (isFullRender) {
        TControlsConfig().isFullRender = false;
    }
}

void TerminalApplication::OnMouseLeftClick(TerminalCoord absPosition, bool isCtrl, bool isFromDoubleClick) {
    auto clickCell = canvas->Get(absPosition);
    auto clickControl = clickCell.GetParent();
    auto clickWindow = clickControl ? clickControl->GetParentWindow() : nullptr;

    SetFocusControl(clickControl, clickWindow);

    bool isDraggingStart = TryDraggingStart(clickControl, absPosition);

    TimeProfiler& tp = TControlsConfig().tp;
    tp.Push("MoveToTop");
    bool isMoveToTop = rootControl->MoveToTop(clickWindow);
    tp.Pop("MoveToTop", isMoveToTop);
    
    bool isApplyClickOK = clickControl ? clickControl->ApplyMouseLeftClick(absPosition) : false;

    if (isMoveToTop || isApplyClickOK || isDraggingStart) {
        TimeProfiler& tp = TControlsConfig().tp;
        tp.Push("FrameRender");
        FrameRender();
        tp.Pop("FrameRender" + std::string(isFromDoubleClick ? "DC " : ""));
        tp.forceShouldNotCommit = true;
        FrameRender();
        tp.forceShouldNotCommit = false;
    }
}

void TerminalApplication::OnMouseDoubleClick(TerminalCoord absPosition, bool isCtrl) {
    OnMouseLeftClick(absPosition, isCtrl, true);
}

void TerminalApplication::OnMouseUp(TerminalCoord absPosition) {
    if (TryDraggingStop()) {
        FrameRender();
    }
}

void TerminalApplication::OnMouseMoved(TerminalCoord absPosition) {
    auto cellUnderMouse = canvas->Get(absPosition);
    auto curUnderMouseControl = cellUnderMouse.GetParent();

    if (TryDragging(absPosition) | TryMouseOver(curUnderMouseControl) | TryMouseOut(curUnderMouseControl)) {
        FrameRender();
    }

    prvUnderMouseControl = curUnderMouseControl;
}

void TerminalApplication::OnMouseWheeled(short value) {
    if (focusControl) {
        if (focusControl->ApplyMouseWheeled(value)) {
            FrameRender();
        }
    }
}

void TerminalApplication::OnKeyPressUpOrDown(bool isUp) {
    if (focusControl) {
        if (focusControl->ApplyKeyPressUpOrDown(isUp)) {
            FrameRender();
        }
    }
}

bool TerminalApplication::TryMouseOver(TerminalControl* curUnderMouseControl) {
    if (curUnderMouseControl == nullptr) {
        return false;
    }
    if (curUnderMouseControl != prvUnderMouseControl) {
        return curUnderMouseControl->ApplyMouseOver();
    }
    return false;
}

bool TerminalApplication::TryMouseOut(TerminalControl* curUnderMouseControl) {
    if (prvUnderMouseControl == nullptr) {
        return false;
    }
    if (prvUnderMouseControl != curUnderMouseControl) {
        return prvUnderMouseControl->ApplyMouseOut();
    }
    return false;
}
bool TerminalApplication::TryDraggingStart(TerminalControl* control, TerminalCoord absPosition) {
    if (control != nullptr && control->IsDraggable() && control->TryDraggingStart(absPosition)) {
        draggingControl = control;
        draggingBasePoint = absPosition;
        return true;
    }
    return false;
}

bool TerminalApplication::TryDragging(TerminalCoord absPosition) {
    if (draggingControl) {
        if (absPosition == draggingBasePoint) {
            return false;
        }
        TerminalCoord delta = absPosition - draggingBasePoint;
        if (draggingControl->TryDragging(delta)) {
            draggingBasePoint = absPosition;
        }
        return true;
    }
    return false;
}

bool TerminalApplication::TryDraggingStop() {
    if (draggingControl) {
        draggingControl->TryDraggingStop();
        draggingControl = nullptr;
        return true;
    }
    return false;
}

void TerminalApplication::SetFocusControl(TerminalControl* clickControl, TerminalWindow* clickWindow) {
    if (clickControl) {
        if (clickControl->IsFocusable()) {
            focusControl = clickControl;
        }
    }
    else if (clickWindow) {
        if (clickWindow->IsFocusable()) {
            focusControl = clickWindow;
        }
    }
}

void TerminalApplication::OnKeyEvent(const KEY_EVENT_RECORD& key) {
    std::string dir = key.bKeyDown ? "Down" : "Up";
    if (key.bKeyDown) {
        if (key.wVirtualKeyCode == VK_UP || key.wVirtualKeyCode == VK_DOWN) {
            OnKeyPressUpOrDown(key.wVirtualKeyCode == VK_UP);
        }
    }
    if (key.bKeyDown && key.uChar.AsciiChar == 'q') {
        exit(0);
    }
}

void TerminalApplication::OnMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent) {
    std::string info;
    TerminalCoord absPosition{
        .row = mouseEvent.dwMousePosition.Y,
        .col = mouseEvent.dwMousePosition.X 
    };
    if (mouseEvent.dwEventFlags != 0) { // just click
        if (mouseEvent.dwEventFlags & DOUBLE_CLICK) {
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                OnMouseDoubleClick(absPosition, mouseEvent.dwControlKeyState& LEFT_CTRL_PRESSED);
            }
            info += "Double click";
        }
        if (mouseEvent.dwEventFlags & MOUSE_MOVED) {
            OnMouseMoved(absPosition);
            info += "Move [" + std::to_string(mouseEvent.dwMousePosition.X) + ", " + std::to_string(mouseEvent.dwMousePosition.Y) + "]";
        }
        if (mouseEvent.dwEventFlags & MOUSE_WHEELED) {
            short value = GET_WHEEL_DELTA_WPARAM(mouseEvent.dwButtonState);
            OnMouseWheeled(value);
            info += "MOUSE_WHEELED: " + std::to_string(value);
        }
        if (mouseEvent.dwEventFlags & MOUSE_HWHEELED) {
            info += "MOUSE_HWHEELED !!!: " + std::to_string(mouseEvent.dwButtonState);
        }
    }
    else {
        if (mouseEvent.dwButtonState == 0) {
            OnMouseUp(absPosition);
            info = "Up";
        }
        if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            OnMouseLeftClick(absPosition, mouseEvent.dwControlKeyState & LEFT_CTRL_PRESSED);
            info = "LeftButton";
        }
        if (mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
            info = "RightButton";
        }
        if (mouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED) {
            info = "FROM_LEFT_2ND_BUTTON_PRESSED";
        }
        if (mouseEvent.dwButtonState == FROM_LEFT_3RD_BUTTON_PRESSED) {
            info = "FROM_LEFT_3RD_BUTTON_PRESSED";
        }
        if (mouseEvent.dwButtonState == FROM_LEFT_4TH_BUTTON_PRESSED) {
            info = "FROM_LEFT_4TH_BUTTON_PRESSED";
        }
    }
    //std::cout << "OnMouseEvent: " << info << std::endl;
}

void TerminalApplication::OnWindowResize(short rows, short cols) {
    canvas->Resize(rows, cols);
    rootControl->Resize(rows, cols);
    FrameRender(true);
}

void TerminalApplication::OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent) {
    OnWindowResize(windowSizeEvent.dwSize.Y, windowSizeEvent.dwSize.X);
    //std::cout << "WINDOW_BUFFER_SIZE_RECORD: " << windowSizeEvent.dwSize.X << " : " << windowSizeEvent.dwSize.Y << std::endl;
}

void TerminalApplication::Run() {
    DWORD cc;
    INPUT_RECORD irec;

    //FrameRender();
    for (;;) {
        ReadConsoleInput(inputHandle, &irec, 1, &cc);
        if (irec.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD& keyEvent = (KEY_EVENT_RECORD&)irec.Event;
            OnKeyEvent(keyEvent);
        }
        if (irec.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD& mouseEvent = (MOUSE_EVENT_RECORD&)irec.Event;
            OnMouseEvent(mouseEvent);
        }
        if (irec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent = (WINDOW_BUFFER_SIZE_RECORD&)irec.Event;
            OnWindowResizeEvent(windowSizeEvent);
        }
    }
}

