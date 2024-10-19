#pragma once

#include "utils/utf8string.h"
#include "TextFormat/FontColor.h"
#include "TextFormat/BackgroundColor.h"
#include "TextFormat/TextStyle.h"

class TerminalCell {
public:
    TerminalCell() = default;
    explicit TerminalCell(const Rune& rune);
    TerminalCell(const Rune& rune, const FontColor& fColor);
    TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor);
    TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor, const TextStyle& tStyle);
    TerminalCell(const TerminalCell& other);

    const TerminalControl* GetParent() const;
    TerminalControl* GetParent();

    void SetParent(TerminalControl* newParent);
    void SetTextStyle(TextStyle textStyle);
    void Render() const;
    bool operator == (const TerminalCell& rhs);
    bool operator != (const TerminalCell& rhs);

private:
    Rune data = Rune(" ");

    TextStyle textStyle = TextStyle::Default;
    FontColor fontColor = FontColor::Default;
    BackgroundColor backgroundColor = BackgroundColor::Default;

    TerminalControl* parent = nullptr;
};
