#include "TerminalLabelFixedWidth.h"

TerminalLabelFixedWidth::TerminalLabelFixedWidth(TerminalCoord position, TerminalSize size)
    : TerminalLabelBase(position, size)
    , text("")
{
    InitRenderText();
}

TerminalLabelFixedWidth::TerminalLabelFixedWidth(Utf8String label, TerminalCoord position)
    : TerminalLabelBase(position, TerminalSize{.height = 1, .width = (short)label.size()})
    , text(label)
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
    text = newText;
    InitRenderText();
}