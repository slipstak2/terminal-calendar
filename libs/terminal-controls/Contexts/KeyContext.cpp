#include "KeyContext.h"
#include <Windows.h>

KeyContext::KeyContext(const KEY_EVENT_RECORD* key)
    :isKeyDown(key->bKeyDown)
{
    if (key->uChar.UnicodeChar != 0) {
        if (key->uChar.UnicodeChar == 8) {
            isBackSpace = true;
        } else {
            rune = Rune(key->uChar.UnicodeChar);
        }
    } else {
        isLeft = key->wVirtualKeyCode == VK_LEFT;
        isRight = key->wVirtualKeyCode == VK_RIGHT;
    }
}