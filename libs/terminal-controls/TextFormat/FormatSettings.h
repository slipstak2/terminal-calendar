#pragma once

#include "BackgroundColor.h"
#include "FontColor.h"
#include "TextStyle.h"
#include "RGB.h"

#include <memory>

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

    std::shared_ptr<RGB> fontRBG = nullptr;
    std::shared_ptr<RGB> backgoundRBG = nullptr;
};