#pragma once

#include "TerminalButton.h"
#include "TerminalCompositeControl.h"

class TerminalCheckBox;
using CheckBoxChangedCallback = std::function<void(TerminalCheckBox* sender, bool isChecked)>;

class TerminalCheckBox : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::CHECK_BOX)

    DECLARE_CREATE(TerminalCheckBox)

    TerminalCheckBox(const Utf8String& label, TerminalCoord position);

public:
    bool SetChecked(bool isCheck);
    bool GetChecked();

    void SetOnChangedCallback(CheckBoxChangedCallback changedCallback) {
        this->changedCallback = changedCallback;
    }

protected:
    CheckBoxChangedCallback changedCallback = nullptr;
protected:
    bool isChecked = false;
    TerminalButtonPtr checkedButton;
    TerminalButtonPtr labelButton;
protected:
    static const Rune CheckedRune;
    static const Rune UncheckedRune;

};