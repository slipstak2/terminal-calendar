#include <Windows.h>
#include "Contexts/MouseContext.h"


MouseContext::MouseContext(const MOUSE_EVENT_RECORD* mouseEvent) {
    isCtrl = (mouseEvent->dwControlKeyState & LEFT_CTRL_PRESSED) || (mouseEvent->dwControlKeyState & RIGHT_CTRL_PRESSED);
    //https://github.com/gui-cs/Terminal.Gui/issues/1848#issuecomment-1173994897
    isDoubleClick = mouseEvent->dwEventFlags & DOUBLE_CLICK;
}