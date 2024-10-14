#include "TerminalCell.h"

void TerminalCell::SetParent(TerminalControl* newParent) {
    parent = newParent;
}

void TerminalCell::SetTextStyle(TextStyle newTextStyle) {
    textStyle = newTextStyle;
}

const TerminalControl* TerminalCell::GetParent() const {
    return parent;
}

TerminalControl* TerminalCell::GetParent() {
    return parent;
}

bool TerminalCell::operator == (const TerminalCell& rhs) {
    return 
        std::tie(data, fontColor, backgroundColor, textStyle, parent) == 
        std::tie(rhs.data, rhs.fontColor, rhs.backgroundColor, rhs.textStyle, rhs.parent);
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
    printf("\033[%d;%d;%dm%s\033[0m", (int)(textStyle), int(backgroundColor), (int)fontColor, data.get());

    // \033[38;2;<r>;<g>;<b>m           #Select RGB foreground color
    // \033[48; 2; <r>; <g>; <b>m       #Select RGB background color
    //printf("\033[38;2;201;200;59\033[48;2;201;100;59m%s", data.get());

    //printf("\033[5;%d;%dm%s\033[0m", BackgroundColor::Red, FontColor::Yellow, data.get());
}

TerminalCell::TerminalCell(const Rune& rune) 
    : data(rune) 
{}

TerminalCell::TerminalCell(const Rune& rune, const FontColor& fColor)
    : data(rune)
    , fontColor(fColor)
{}

TerminalCell::TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor)
    : data(rune)
    , fontColor(fColor)
    , backgroundColor(bColor)
{}

TerminalCell::TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor, const TextStyle& tStyle)
    : data(rune)
    , fontColor(fColor)
    , backgroundColor(bColor)
    , textStyle(tStyle)
{}

TerminalCell::TerminalCell(const TerminalCell& other)
    : data(other.data)
    , fontColor(other.fontColor)
    , backgroundColor(other.backgroundColor)
    , textStyle(other.textStyle)
    , parent(other.parent)
{}