#pragma once

#include "utils/utf8string.h"
#include "TextFormat/FontColor.h"
#include "TextFormat/BackgroundColor.h"
#include "TextFormat/TextStyle.h"
#include "TextFormat/FormatSettings.h"

class TerminalCell {
public:
    TerminalCell() = default;
    explicit TerminalCell(const Rune& rune);
    TerminalCell(const Rune& rune, const FormatSettings* formatSettings);
    TerminalCell(const TerminalCell& other);

    const TerminalControl* GetParent() const;
    TerminalControl* GetParent();

    void SetParent(TerminalControl* newParent);
    void SetFormatSettings(const FormatSettings* formatSettings);
    const FormatSettings& GetFormatSettings() const;
    void Render() const;
    bool operator == (const TerminalCell& rhs);
    bool operator != (const TerminalCell& rhs);

private:
    Rune rune = Rune(" ");

    TerminalControl* parent = nullptr;
    const FormatSettings* formatSettings = nullptr;
};
