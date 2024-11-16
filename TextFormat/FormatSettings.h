#pragma once

#include "TextFormat/BackgroundColor.h"
#include "TextFormat/FontColor.h"
#include "TextFormat/TextStyle.h"

class FormatSettings {
public:
    static const FormatSettings Default;
    static const FormatSettings ScrollDefault;
    static const FormatSettings ScrollDragging;
public:
    bool operator == (const FormatSettings& rhs) const;
    bool IsDefault() const;
public:
    BackgroundColor backgroundColor = BackgroundColor::Default;
    FontColor fontColor = FontColor::Default;
    TextStyle textStyle = TextStyle::Default;
};