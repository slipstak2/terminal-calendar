#include "TerminalCheckBox.h"
#include "utf8string.h"

const Utf8String TerminalCheckBox::CheckedTitle   = "▣ ";// ☒ ☑ ▣
const Utf8String TerminalCheckBox::UncheckedTitle = "□ ";

TerminalCheckBox::TerminalCheckBox(const Utf8String& label, TerminalCoord position, bool withCheckedBox)
    : TerminalCompositeControl(position, TerminalSize{ .height = 1, .width = (short)label.size() + (withCheckedBox ? 2 : 0) })
{
    auto clickCallBack = [this](const MouseContext& ctx) {
        return SetChecked(ctx, !GetChecked());
    };

    if (withCheckedBox) {
        checkedButton = TerminalButton::Create(UncheckedTitle, TerminalCoord{ .row = 0, .col = 0 });
        checkedButton->AddClickCallback(clickCallBack);
        AddControl(checkedButton);
    }

    labelButton = TerminalButton::Create(label, TerminalCoord{ .row = 0, .col = (withCheckedBox ? 2 : 0) });
    labelButton->AddClickCallback(clickCallBack);
    AddControl(labelButton);

    // TODO: changed prototype for AddOnChangedCallback and AddMouseOverCallback|AddMouseOutCallback

    // how to join callbacks?
    if (checkedButton) {
        checkedButton->AddMouseOverCallback([this]() {
            return ProccessDefaultMouseOver();
        });
        checkedButton->AddMouseOutCallback([this]() {
            return ProccessDefaultMouseOut();
        });
        checkedButton->SetFontColor(noSelectedColor);
    }

    labelButton->AddMouseOverCallback([this]() {
        return ProccessDefaultMouseOver();
    });

    labelButton->AddMouseOutCallback([this]() {
        return ProccessDefaultMouseOut();
    });

    labelButton->SetFontColor(noSelectedColor);
}

bool TerminalCheckBox::SetChecked(bool isCheck, bool useSubcribeCallbacks) {
    return SetChecked(MouseContext(), isCheck, useSubcribeCallbacks);
}

bool TerminalCheckBox::SetChecked(const MouseContext& ctx, bool isChecked, bool useSubcribeCallbacks) {
    bool isChanged = this->isChecked != isChecked;
    this->isChecked = isChecked;
    if (checkedButton) {
        checkedButton->SetText(this->isChecked ? CheckedTitle : UncheckedTitle);
    }

    if (isChanged) {
        ProccessDefaultChanged(useSubcribeCallbacks);
        if (useSubcribeCallbacks) {
            for (auto& changedCallback : changedCallbacks) {
                changedCallback(ctx, this, isChecked);
            }
        }
    }
    return isChanged;
}

bool TerminalCheckBox::GetChecked() {
    return isChecked;
}

void TerminalCheckBox::SetLabelFormatSettings(FormatSettings labelFormatSettings) {
    labelButton->SetFormatSettings(labelFormatSettings);
}

TerminalButtonPtr TerminalCheckBox::GetCheckedButton() {
    return checkedButton;
}

TerminalButtonPtr TerminalCheckBox::GetLabelButton() {
    return labelButton;
}

bool TerminalCheckBox::ProccessDefaultMouseOver() {
    if (!GetChecked()) {
        labelButton->SetFontColor(mouseOverColor);
        if (checkedButton) {
            checkedButton->SetFontColor(mouseOverColor);
        }
    }
    return true;
}

bool TerminalCheckBox::ProccessDefaultMouseOut() {
    if (!GetChecked()) {
        labelButton->SetFontColor(noSelectedColor);
        if (checkedButton) {
            checkedButton->SetFontColor(noSelectedColor);
        }
    }
    return true;
}

void TerminalCheckBox::ProccessDefaultChanged(bool useSubcribeCallbacks) {
    FontColor fontColor = useSubcribeCallbacks ? mouseOverColor : noSelectedColor;
    if (isChecked) {
        fontColor = selectedColor;
    }
    labelButton->SetFontColor(fontColor);
    if (checkedButton) {
        checkedButton->SetFontColor(fontColor);
    }
}