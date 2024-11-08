#include "TerminalLabel.h"

TerminalLabel::TerminalLabel(const Utf8String& label, TerminalCoord position)
    : TerminalLabelBase(position, { .height = 1, .width = (short)label.size() })
    , text(label)
{}

const Utf8String& TerminalLabel::Get() const {
    return text;
}

short TerminalLabel::Length() const {
    return (short)text.size();
}

void TerminalLabel::SetText(const Utf8String& newText) {
    assert(newText.size() <= Width());
    text = newText;
}