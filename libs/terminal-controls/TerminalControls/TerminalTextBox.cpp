#include "TerminalTextBox.h"
#include "TerminalLabel.h"

TerminalTextBox::TerminalTextBox(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
    , renderTextView(text, 0, size.width - 1)
{
    AddKeyPressCallbacks([this](const KeyContext& kctx) {
        if (!kctx.rune.is_empty()) {
            text.push_back(kctx.rune); // TODO: tabulation saw strange
            renderTextView.tryIncOffset();
            SetCursorPosition();
            return true;
        }
        if (kctx.isBackSpace) {
            if (text.empty()) {
                return false;
            }
            text.pop_back();
            renderTextView.tryDecOffset();
            SetCursorPosition();
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

    SetCursorPosition();
}

void TerminalTextBox::SetCursorPosition() {
    cursor->SetPosition(TerminalCoord{.row = 0, .col = (short)renderTextView.size() });
}

void TerminalTextBox::FlushSelf() {

    size_t viewSize = renderTextView.size();

    short len = Width();
    for (int i = 0; i < len; ++i) {
        if (i < viewSize) {
            data[0][i] = CreateCell(renderTextView[i]);
        } else {
            data[0][i] = CreateCell(" ");
        }
    }
}
