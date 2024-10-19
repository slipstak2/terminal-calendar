#include "TerminalLabel.h"
#include <cassert>

TerminalLabel::TerminalLabel(const Utf8String& label, TerminalCoord position)
    : TerminalLabelBase(position, { .height = 1, .width = (short)label.size() })
    , text(label)
{}

const Utf8String& TerminalLabel::Get() const {
    return text;
}

short TerminalLabel::Size() const {
    return (short)text.size();
}

void TerminalLabel::SetText(const Utf8String& newText) { // TODO: Separate TerminalLabel and TerminalDataProviderLabel
    assert(newText.size() <= text.size());
    text = newText;
}