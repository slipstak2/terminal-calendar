#include "FormatSettings.h"
#include <tuple>

const FormatSettings FormatSettings::Default;
const FormatSettings FormatSettings::ScrollDefault = FormatSettings{.fontColor = FontColor::Brightwhite};
const FormatSettings FormatSettings::ScrollDragging = FormatSettings{ .fontColor = FontColor::Brightblue};

bool FormatSettings::operator == (const FormatSettings& rhs) const {
    return 
        std::tie(fontColor, backgroundColor, textStyle) == 
        std::tie(rhs.fontColor, rhs.backgroundColor, rhs.textStyle);
}