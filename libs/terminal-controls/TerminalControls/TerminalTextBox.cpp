#include "TerminalTextBox.h"

TerminalTextBox::TerminalTextBox(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
{
    AddKeyPressCallbacks([this](const KeyContext& kctx) {
        if (!kctx.rune.is_empty()) {
            text.push_back(kctx.rune);
            InitRenderText();
            return true;
        }
        return false;
        });
    InitRenderText();
}

void TerminalTextBox::InitRenderText() {
    renderText = text;
    renderText.resize(Width(), ' ');
}

void TerminalTextBox::FlushSelf() {
    FlushSelfAllignLeft(renderText);
}

void TerminalTextBox::FlushSelfAllignLeft(const Utf8String& s) {
    short len = Width();
    for (int i = 0; i < len; ++i) {
        if (i < s.size()) {
            data[0][i] = CreateCell(s[i]);
        }
        else {
            data[0][i] = CreateBackgroundCell(" ");
        }
    }
}