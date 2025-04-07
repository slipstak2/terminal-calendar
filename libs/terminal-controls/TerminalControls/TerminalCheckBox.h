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

    void AddOnChangedCallback(CheckBoxChangedCallback changedCallback) {
        changedCallbacks.push_back(changedCallback);
    }
    void SetLabelFormatSettings(FormatSettings labelFormatSettings);

    TerminalButtonPtr GetCheckedButton();
    TerminalButtonPtr GetLabelButton();

protected:
    std::vector<CheckBoxChangedCallback> changedCallbacks;
protected: // TODO: return protected
    bool isChecked = false;
    TerminalButtonPtr checkedButton;
    TerminalButtonPtr labelButton;
protected:
    static const Utf8String CheckedTitle;
    static const Utf8String UncheckedTitle;
};