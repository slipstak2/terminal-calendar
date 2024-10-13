#pragma once

#include "utils/utf8string.h"
#include "FontColor.h"
#include "BackgroundColor.h"

class TerminalCell {
public:
    TerminalCell() = default;
    explicit TerminalCell(const Rune& rune);
    TerminalCell(const Rune& rune, const FontColor& fColor);
    TerminalCell(const Rune& rune, const FontColor& fColor, const BackgroundColor& bColor);
    TerminalCell(const TerminalCell& other);

    const TerminalControl* GetParent() const;
    TerminalControl* GetParent();

    void SetParent(TerminalControl* newParent);
    void Render() const;
    bool operator == (const TerminalCell& rhs);
    bool operator != (const TerminalCell& rhs);

private:
    Rune data = Rune(" ");


    FontColor fontColor = FontColor::Default;
    BackgroundColor backgroundColor = BackgroundColor::Default;

    TerminalControl* parent = nullptr;
};
