#pragma once

#include "rune.h"

struct _KEY_EVENT_RECORD;
typedef _KEY_EVENT_RECORD KEY_EVENT_RECORD;


struct KeyContext {
    KeyContext() = default;
    KeyContext(const KEY_EVENT_RECORD* key);

    bool isBackSpace = false;
    bool isDelete = false;
    bool isKeyDown = false;

    bool isLeft = false;
    bool isRight = false;
    Rune rune;
};
