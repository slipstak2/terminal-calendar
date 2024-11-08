#include "TerminalLabelFixedWidth.h"

TerminalLabelFixedWidth::TerminalLabelFixedWidth(TerminalCoord position, TerminalSize size)
    : TerminalLabelBase(position, size)
    , text("")
{
    InitRenderText();
}

void TerminalLabelFixedWidth::InitRenderText() {
    renderText = text;
    renderText.resize(Width(), ' ');
}

const Utf8String& TerminalLabelFixedWidth::Get() const {
    return renderText;
}

short TerminalLabelFixedWidth::Length() const {
    return Width();
}

void TerminalLabelFixedWidth::SetText(const Utf8String& newText) {
    assert(newText.size() <= Width());
    text = newText;
    InitRenderText();
}