#include "TerminalCheckBox.h"
#include "utils/utf8string.h"

const Rune TerminalCheckBox::CheckedRune = Rune("▣"); // ☒ ☑ ▣
const Rune TerminalCheckBox::UncheckedRune = Rune("□"); // ☐


TerminalCheckBox::TerminalCheckBox(const Utf8String& label, TerminalCoord position)
    : TerminalCompositeControl(position, TerminalSize{ .height = 1, .width = (short)label.size() + 2 })
{
    auto clickCallBack = [this]() {
        return SetChecked(!GetChecked());
        };

    checkedButton = TerminalButton::Create(Utf8String(UncheckedRune) + " ", TerminalCoord{ .row = 0, .col = 0 });
    checkedButton->AddClickCallback(clickCallBack);
    AddControl(checkedButton);

    labelButton = TerminalButton::Create(label, TerminalCoord{ .row = 0, .col = ONE + ONE });
    labelButton->AddClickCallback(clickCallBack);
    AddControl(labelButton);

    formatSettings.backgroundColor = BackgroundColor::Yellow;
}

bool TerminalCheckBox::SetChecked(bool isChecked) {
    bool isChanged = this->isChecked != isChecked;
    this->isChecked = isChecked;
    checkedButton->SetText(this->isChecked ? Utf8String(CheckedRune) : Utf8String(UncheckedRune) + " ");

    if (isChanged) {
        if (changedCallback != nullptr) {
            changedCallback(this, isChecked);
        }
    }
    return isChanged;
}

bool TerminalCheckBox::GetChecked() {
    return isChecked;
}
