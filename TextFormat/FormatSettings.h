#pragma once

#include "TextFormat/BackgroundColor.h"
#include "TextFormat/FontColor.h"
#include "TextFormat/TextStyle.h"

class FormatSettings {
    BackgroundColor backgroundColor = BackgroundColor::Default;
    FontColor fontColor = FontColor::Default;
    TextStyle textStyle = TextStyle::Default;
};