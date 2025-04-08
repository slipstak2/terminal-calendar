#pragma once

struct _MOUSE_EVENT_RECORD;
typedef _MOUSE_EVENT_RECORD MOUSE_EVENT_RECORD;

struct MouseContext {
    MouseContext() = default;
    MouseContext(const MOUSE_EVENT_RECORD* mouseEvent);

    bool isCtrl = false;
    bool isDoubleClick = false;
};
