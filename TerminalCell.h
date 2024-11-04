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

    const TerminalControl* GetParent() const;
    TerminalControl* GetParent();

    void SetParent(TerminalControl* newParent);
    void SetFormatSettings(const FormatSettings* formatSettings);
    const FormatSettings& GetFormatSettings() const;
    void Render() const;
    bool operator == (const TerminalCell& rhs);
    bool operator != (const TerminalCell& rhs);
protected:
    struct TerminalCellSnapshot {
        Rune rune;
        FormatSettings formatSettings;
        bool operator == (const TerminalCellSnapshot& rhs) {
            return std::tie(rune, formatSettings) == std::tie(rhs.rune, rhs.formatSettings);
        }
    };
public:
    void MakeSnapshot() {
        snapshot = TerminalCellSnapshot{ .rune = rune, .formatSettings = GetFormatSettings()};
    }

private:
    Rune rune = Rune(" ");
    const FormatSettings* formatSettings = nullptr;
    TerminalControl* parent = nullptr;

    TerminalCellSnapshot snapshot;
};
