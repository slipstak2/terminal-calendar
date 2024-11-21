#pragma once

#include "utf8string.h"
#include "TextFormat/FormatSettings.h"

class TerminalControl;

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
public:
    struct TerminalCellSnapshot {
        Rune rune;
        FormatSettings formatSettings;
        bool operator == (const TerminalCell& rhs) const {
            return std::tie(rune, formatSettings) == std::tie(rhs.rune, rhs.GetFormatSettings());
        }
        bool operator != (const TerminalCell& rhs) const {
            return !(*this == rhs);
        }
    };
public:
    void MakeSnapshot() {
        snapshot = TerminalCellSnapshot{ .rune = rune, .formatSettings = GetFormatSettings()};
    }
    const TerminalCellSnapshot& GetSnapshot() {
        return snapshot;
    }

private:
    Rune rune = Rune(" ");
    const FormatSettings* formatSettings = nullptr;
    TerminalControl* parent = nullptr;

    TerminalCellSnapshot snapshot;
};
