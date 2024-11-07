#include "TerminalLabel.h"

TerminalLabel::TerminalLabel(const Utf8String& label, TerminalCoord position)
    : TerminalLabelBase(position, { .height = 1, .width = (short)label.size() })
    , text(label)
{}

TerminalLabel::TerminalLabel(TerminalCoord position, TerminalSize size)
    : TerminalLabelBase(position, size)
    , text(Utf8String::Empty)
{}

const Utf8String& TerminalLabel::Get() const {
    return text;
}

short TerminalLabel::Length() const {
    return (short)text.size();
}

void TerminalLabel::SetText(const Utf8String& newText) { // TODO: Separate TerminalLabel and TerminalDataProviderLabel
    assert(newText.size() <= Width());
    text = newText;
}