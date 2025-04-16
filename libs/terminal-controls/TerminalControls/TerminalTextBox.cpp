#include "TerminalTextBox.h"

TerminalTextBox::TerminalTextBox(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
{
    AddKeyPressCallbacks([this](const KeyContext& kctx) {
        if (!kctx.rune.is_empty()) {
            text.push_back(kctx.rune); // TODO: tabulation saw strange
            InitRenderText();
            return true;
        }
        if (kctx.isBackSpace) {
            if (text.empty()) {
                return false;
            }
            text.pop_back();
            InitRenderText();
            return true;
        }
        return false;
        });
    auto onChangeFocus = [this](TerminalControl* sender) {
        SetBackgroundColor(IsFocus() ? BackgroundColor::Magenta : BackgroundColor::Brightblack);
    };
    AddChangeFocusCallbacks(onChangeFocus);
    onChangeFocus(this);

    InitRenderText();
}

void TerminalTextBox::InitRenderText() {
    renderText = text;
    renderText.resize_last(Width(), ' ');
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