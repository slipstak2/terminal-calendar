#pragma once

#include "TerminalControl.h"


class TerminalTextBox : public TerminalControl { // TODO; TerminalControl + keyinput

    class TextView {
    public:
        TextView(Utf8String& text, size_t offset, size_t len)
            : text(text)
            , offset(offset)
            , len(len)
        {}
        size_t size() const {
            if (offset + len <= text.size()){
                return len;
            } 
            if (offset < text.size()) {
                return text.size() - offset;
            }
            return 0;
        }
        const Rune& operator [] (size_t idx) const {
            if (offset + idx < text.size()) {
                return text[offset + idx];
            }
            static Rune empty("?");
            return empty;
        }
        size_t Offset() const {
            return offset;
        }
        void SetOffset(size_t offset) {
            this->offset = offset;
        }
    private:

        Utf8String& text;
        size_t offset;
        size_t len;
    };
public:
    DECLARE_CREATE(TerminalTextBox)
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::TEXT_BOX)

public:
    TerminalTextBox(TerminalCoord position, TerminalSize size);
    void SetText(const Utf8String& text);
protected:
    void FlushSelf() override;

protected:
    void AddSymbol(Rune r);
    bool RemoveCurSymbol();
    bool RemovePrevSymbol();
    bool TryMoveCursor(int delta);

    void NormalizeView();
    void UpdateCursorLabelPosition(bool isVisible = true);
protected:
    Utf8String text;
    int cursorPosition = 0;
    TextView textView;

    BackgroundColor focusBackgroundColor = BackgroundColor::Magenta;
    BackgroundColor notFocusBackgroundColor = BackgroundColor::Brightblack;

    TerminalLabelPtr cursorLabel = nullptr;
};