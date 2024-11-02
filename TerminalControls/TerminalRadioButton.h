#include "TerminalButton.h"
#include "TerminalCompositeControl.h"

class TerminalRadioButton;
using RadioButtonChangedCallback = std::function<void(TerminalRadioButton* sender)>;

class TerminalRadioButton : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::RADIO_BUTTON)

    DECLARE_CREATE(TerminalRadioButton)

    TerminalRadioButton(const Utf8String& label, TerminalCoord position);

public:
    bool SetEnable(bool isEnable);
    bool GetEnable();

    void SetOnChangedCallback(RadioButtonChangedCallback changedCallback) {
        this->changedCallback = changedCallback;
    }

protected:
    RadioButtonChangedCallback changedCallback = nullptr;
protected:
    bool isEnable = false;
    TerminalButtonPtr radioButton;
    TerminalButtonPtr labelButton;
protected:
    static const Rune EnableRune;
    static const Rune DisableRune;
};