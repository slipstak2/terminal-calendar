#include <Windows.h>
#include "Contexts/MouseContext.h"


MouseContext::MouseContext(const MOUSE_EVENT_RECORD* mouseEvent) {
    isCtrl = (mouseEvent->dwControlKeyState & LEFT_CTRL_PRESSED) || (mouseEvent->dwControlKeyState & RIGHT_CTRL_PRESSED);
    isDoubleClick = mouseEvent->dwEventFlags & DOUBLE_CLICK;
}