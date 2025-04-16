#include "KeyContext.h"
#include <Windows.h>

KeyContext::KeyContext(const KEY_EVENT_RECORD* key)
    :isKeyDown(key->bKeyDown)
{
    if (key->uChar.UnicodeChar != 0) {
        //rune = Rune(key->uChar.UnicodeChar);
    }
}