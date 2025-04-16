#pragma once

#include "rune.h"

struct _KEY_EVENT_RECORD;
typedef _KEY_EVENT_RECORD KEY_EVENT_RECORD;


struct KeyContext {
    KeyContext() = default;
    KeyContext(const KEY_EVENT_RECORD* key);

    bool isKeyDown = false;
    Rune rune;
};
