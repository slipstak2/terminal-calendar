#pragma once

#include "utils/utf8string.h"

class BorderFormat {
public:
    static BorderFormat Default;
    static BorderFormat GroupBox;
    static BorderFormat Window;

    BorderFormat(Utf8String& format)
        : s(format)
    {}

    Rune Vertical() { return s[0]; }
    Rune Horizontal() { return s[1]; }

    Rune UpLeft() { return s[2]; }
    Rune UpRight() { return s[3]; }
    Rune BottomLeft() { return s[4]; }
    Rune BottomRight() { return s[5]; }

protected:
    Utf8String s;
};
