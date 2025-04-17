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
        bool tryIncOffset() {
            if (offset + len + 1 == text.size()) {
                offset++;
                return true;
            }
            return false;
        }
        bool tryDecOffset() {
            if (offset + len - 1 == text.size()) {
                if (offset != 0) {
                    offset--;
                }
                return true;
            }
            return false;
        }
    private:

        Utf8String& text;
        size_t offset;
        size_t len;
    };
    class Cursor {

        int pos = -1;
    };
public:
    DECLARE_CREATE(TerminalTextBox)
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::TEXT_BOX)

public:
    TerminalTextBox(TerminalCoord position, TerminalSize size);

protected:
    void FlushSelf() override;

protected:
    void SetCursorPosition();
protected:
    Utf8String text;
    TextView renderTextView;

    BackgroundColor focusBackgroundColor = BackgroundColor::Magenta;
    BackgroundColor notFocusBackgroundColor = BackgroundColor::Brightblack;

    TerminalLabelPtr cursor = nullptr;
};