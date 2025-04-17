#include "TerminalTextBox.h"
#include "TerminalLabel.h"

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
        SetBackgroundColor(IsFocus() ? focusBackgroundColor : notFocusBackgroundColor);
        if (IsFocus()) {
            AddControl(cursor);
        }
        else {
            size_t removeControls = RemoveControls(GetControls(), [](TerminalControlPtr control) {
                return control->As<TerminalLabel>() != nullptr;
            });
            assert(removeControls <= 1);
        }
    };
    AddChangeFocusCallbacks(onChangeFocus);
    onChangeFocus(this);


    cursor = TerminalLabel::Create("_", TerminalCoord{ .row = 0, .col = 0 }); // (short)renderText.size()
    cursor->SetBackgroundColor(focusBackgroundColor);
    cursor->SetTextStyle(TextStyle::RapidBlink);

    InitRenderText();
}

void TerminalTextBox::InitRenderText() {
    renderText = text;
    renderText.resize_last(Width() - 1);
    cursor->SetPosition(TerminalCoord{.row = 0, .col = (short)renderText.size() });
}

void TerminalTextBox::FlushSelf() {
    const Utf8String& s = renderText;

    short len = Width();
    for (int i = 0; i < len; ++i) {
        if (i < s.size()) {
            data[0][i] = CreateCell(s[i]);
        }
        else {
            data[0][i] = CreateCell(" ");
        }
    }
}
