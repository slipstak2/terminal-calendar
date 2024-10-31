#include "TerminalButton.h"
#include "TerminalCompositeControl.h"

class TerminalRadioButton : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::RADIO_BUTTON)

    DECLARE_CREATE(TerminalRadioButton)

    TerminalRadioButton(const Utf8String& label, TerminalCoord position);

public:
    bool SetEnable(bool isEnable);

protected:
    bool isEnable = false;
    TerminalButtonPtr radioButton;
    TerminalButtonPtr labelButton;
protected:
    static const Rune EnableRune;
    static const Rune DisableRune;
};