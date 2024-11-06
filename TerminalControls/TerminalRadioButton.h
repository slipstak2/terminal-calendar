#include "TerminalButton.h"
#include "TerminalCompositeControl.h"

class TerminalRadioButton;
using RadioButtonChangedCallback = std::function<void(TerminalRadioButton* sender, bool isSelected)>;

class TerminalRadioButton : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::RADIO_BUTTON)

    DECLARE_CREATE(TerminalRadioButton)

    TerminalRadioButton(const Utf8String& label, TerminalCoord position);

public:
    bool SetSelected(bool isSelect);
    bool GetSelected();

    void SetOnChangedCallback(RadioButtonChangedCallback changedCallback) {
        this->changedCallback = changedCallback;
    }

protected:
    RadioButtonChangedCallback changedCallback = nullptr;
protected:
    bool isSelected = false;
    TerminalButtonPtr radioButton;
    TerminalButtonPtr labelButton;
protected:
    static const Utf8String SelectedTitle;
    static const Utf8String UnselectedTitle;
};