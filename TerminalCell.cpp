#include "TerminalCell.h"
#include "TerminalControl.h"

void TerminalCell::SetParent(TerminalControl* newParent) {
    parent = newParent;
}

const FormatSettings& TerminalCell::GetFormatSettings() const {
    if (formatSettings) {
        return *formatSettings;
    }
    if (parent) {
        return parent->GetFormatSettings();
    }
    return FormatSettings::Default;
}

void TerminalCell::SetFormatSettings(const FormatSettings* newFormatSettings) {
    formatSettings = newFormatSettings;
}

const TerminalControl* TerminalCell::GetParent() const {
    return parent;
}

TerminalControl* TerminalCell::GetParent() {
    return parent;
}

bool TerminalCell::operator == (const TerminalCell& rhs) {
    bool isEqual =  
        std::tie(rune, parent, formatSettings) ==
        std::tie(rhs.rune, rhs.parent, rhs.formatSettings);
    if (!isEqual) {
        return false;
    }
    if (formatSettings && rhs.formatSettings) {
        return *formatSettings == *rhs.formatSettings;
    }
    return false;
}

bool TerminalCell::operator != (const TerminalCell& rhs) {
    return !(*this == rhs);
}

// https://habr.com/ru/articles/119436/
// 
// 
//   SetConsoleTextAttribute(hConsole,
// FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_BLUE);
// 
// https://habr.com/ru/articles/119436/
// https://habr.com/ru/companies/macloud/articles/558316/
void TerminalCell::Render() const {
    auto fmtSettings = GetFormatSettings();
    
    printf("\033[%d;%d;%dm%s\033[0m", 
        (int)(fmtSettings.textStyle), 
        int(fmtSettings.backgroundColor), 
        (int)fmtSettings.fontColor, 
        rune.get()
    );

    // \033[53
    // \033[38;2;201;200;59m    полный спектр rbg для цвета и фона
    // \033[38;5;120            256 цветов для шрифта и фона


    // true color + text style (underline)
    //printf("\033[%dm", (int)(fmtSettings.textStyle));
    //printf("\033[38;2;201;200;59m"); // fontColor;
    //printf("%s\033[0m", rune.get());


    // \033[38;2;<r>;<g>;<b>m           #Select RGB foreground color
    // \033[48; 2; <r>; <g>; <b>m       #Select RGB background color
    //printf("\033[38;2;201;200;59\033[48;2;201;100;59m%s", data.get());

    //printf("\033[5;%d;%dm%s\033[0m", BackgroundColor::Red, FontColor::Yellow, data.get());
}

TerminalCell::TerminalCell(const Rune& rune) 
    : rune(rune)
{}

TerminalCell::TerminalCell(const Rune& rune, const FormatSettings* formatSettings)
    : rune(rune)
    , formatSettings(formatSettings)
{}


TerminalCell::TerminalCell(const TerminalCell& other)
    : rune(other.rune)
    , parent(other.parent)
    , formatSettings(other.formatSettings)
{}