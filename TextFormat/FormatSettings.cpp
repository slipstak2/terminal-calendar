#include "FormatSettings.h"
#include <tuple>

FormatSettings FormatSettings::Default;

bool FormatSettings::operator == (const FormatSettings& rhs) const {
    return 
        std::tie(fontColor, backgroundColor, textStyle) == 
        std::tie(rhs.fontColor, rhs.backgroundColor, rhs.textStyle);
}