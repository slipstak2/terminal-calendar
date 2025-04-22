#include "TerminalTextBox.h"
#include "TerminalLabel.h"

TerminalTextBox::TerminalTextBox(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
    , textView(text, 0, size.width - 1)
{
    cursorLabel = TerminalLabel::Create("_", TerminalCoord{ .row = 0, .col = 0 }); // (short)renderText.size()
    cursorLabel->SetBackgroundColor(focusBackgroundColor);
    cursorLabel->SetTextStyle(TextStyle::RapidBlink);
    AddControl(cursorLabel);

    AddKeyPressCallbacks([this](const KeyContext& kctx) {
        bool isChanged = false;
        if (!kctx.rune.is_empty()) {
            AddSymbol(kctx.rune);
            isChanged = true;
        } else if (kctx.isBackSpace) {
            isChanged = RemovePrevSymbol();
        } else if (kctx.isDelete) {
            isChanged = RemoveCurSymbol();
        }
        if (kctx.isLeft) {
            return TryMoveCursor(-1);
        }
        if (kctx.isRight) {
            return TryMoveCursor(1);
        }
        if (isChanged) {
            OnTextChanged();
        }
        return isChanged;
        });
    auto onChangeFocus = [this](TerminalControl* sender) {
        SetBackgroundColor(IsFocus() ? focusBackgroundColor : notFocusBackgroundColor);
        if (IsFocus()) {
            cursorPosition = (int)text.size();
            NormalizeView();
            UpdateCursorLabelPosition();
        }
        else {
            cursorPosition = -1;
            UpdateCursorLabelPosition(false);
        }
    };
    AddChangeFocusCallbacks(onChangeFocus);
    onChangeFocus(this);
}

void TerminalTextBox::SetText(const Utf8String& text) {
    bool isChanged = this->text != text;
    if (isChanged) {
        this->text = text;
        OnTextChanged();
    }
}

const Utf8String& TerminalTextBox::GetText() const {
    return text;
}

void TerminalTextBox::AddTextChangeCallback(TextChangeCallback textChangeCallback) {
    textChangeCallbacks.push_back(textChangeCallback);
}

void TerminalTextBox::NormalizeView() {
    if (cursorPosition < textView.Offset()) {
        textView.SetOffset(cursorPosition);
        return;
    } else    if (cursorPosition == text.size()) {
        textView.SetOffset(std::max(0, cursorPosition - (int)Width() + 1));
        return;
    } else  if (textView.Offset() + textView.size() <= cursorPosition) {
        textView.SetOffset(cursorPosition - textView.size() + 1);
    }

    if (textView.size() < Width() - 1) {
        int delta_offset = Width() - 1 - textView.size();
        textView.SetOffset(std::max(0, (int)textView.Offset() - delta_offset));
    }
}

void TerminalTextBox::UpdateCursorLabelPosition(bool isVisible) {
    short col = Width() + 1;
    if (isVisible) {
        if (cursorPosition == text.size()) {
            col = (short)textView.size();
        }
    }
    cursorLabel->SetPosition(TerminalCoord{ .row = 0, .col = col });
}

void TerminalTextBox::AddSymbol(Rune r) {
    text.insert(cursorPosition++, r);
    NormalizeView();
    UpdateCursorLabelPosition();
}

bool TerminalTextBox::RemoveCurSymbol() {
    if (cursorPosition >= text.size()) {
        return false;
    }
    text.erase(cursorPosition);
    NormalizeView();
    UpdateCursorLabelPosition();
    return true;
}
bool TerminalTextBox::RemovePrevSymbol() {
    if (text.empty() || cursorPosition == 0) {
        return false;
    }
    text.erase(--cursorPosition);
    NormalizeView();
    UpdateCursorLabelPosition();
    return true;
}

bool TerminalTextBox::TryMoveCursor(int delta) {
    cursorPosition += delta;
    if (cursorPosition < 0) {
        cursorPosition = 0;
    }
    if (cursorPosition > text.size()) {
        cursorPosition = (int)text.size();
    }
    NormalizeView();
    UpdateCursorLabelPosition();
    return true; // TOOD: fix it
}

void TerminalTextBox::OnTextChanged() const {
    for (const auto& textChangeCallback : textChangeCallbacks) {
        textChangeCallback(this);
    }
}
void TerminalTextBox::FlushSelf() {

    size_t viewSize = textView.size();
    
    static FormatSettings FS{
        .backgroundColor = focusBackgroundColor,
        .textStyle = TextStyle::Inverse
    };

    short len = Width();
    for (int i = 0; i < len; ++i) {
        if (i < viewSize) {
            bool isUnderCursor = textView.Offset() + i == cursorPosition;
            data[0][i] = CreateCell(textView[i]);
            if (isUnderCursor) {
                data[0][i].SetFormatSettings(&FS);
            }
        } else {
            data[0][i] = CreateCell(" ");
        }
    }
}
