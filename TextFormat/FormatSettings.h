#pragma once

#include "TextFormat/BackgroundColor.h"
#include "TextFormat/FontColor.h"
#include "TextFormat/TextStyle.h"

class FormatSettings {
public:
    static FormatSettings Default;
public:
    bool operator == (const FormatSettings& rhs) const;
public:
    BackgroundColor backgroundColor = BackgroundColor::Default;
    FontColor fontColor = FontColor::Default;
    TextStyle textStyle = TextStyle::Default;
};