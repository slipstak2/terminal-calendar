#include "TerminalCell.h"

void TerminalCell::SetParent(TerminalControl* newParent) {
    parent = newParent;
}

const TerminalControl* TerminalCell::GetParent() const {
    return parent;
}

TerminalControl* TerminalCell::GetParent() {
    return parent;
}

bool TerminalCell::operator == (const TerminalCell& rhs) {
    return 
        std::tie(data, fontColor, backgroundColor, parent) == 
        std::tie(rhs.data, rhs.fontColor, rhs.backgroundColor, rhs.parent);
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
    printf("\033[10;%d;%dm%s\033[0m", (int)backgroundColor, (int)fontColor, data.get());
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

TerminalCell::TerminalCell(const TerminalCell& other)
    : data(other.data)
    , fontColor(other.fontColor)
    , backgroundColor(other.backgroundColor)
    , parent(other.parent)
{}