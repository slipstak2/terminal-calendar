#include "TerminalRadioButton.h"
#include "utils/utf8string.h"

// https://cloford.com/resources/charcodes/utf-8_geometric.htm
// https://unicode.org/charts/PDF/U25A0.pdf
const Rune TerminalRadioButton::EnableRune  = Rune("◉"); // Rune("▣");
const Rune TerminalRadioButton::DisableRune = Rune("◯"); // Rune("□");

TerminalRadioButton::TerminalRadioButton(const Utf8String& label, TerminalCoord position)
    : TerminalCompositeControl(position, TerminalSize{.height = 1, .width = (short)label.size() + 2})
{
    auto clickCallBack = [this]() {
        return SetEnable(true);
        };
    
    radioButton = TerminalButton::Create(Utf8String(DisableRune) + " ", TerminalCoord{.row = 0, .col = 0});
    radioButton->AddClickCallback(clickCallBack);
    AddControl(radioButton);
    
    labelButton = TerminalButton::Create(label, TerminalCoord{ .row = 0, .col = ONE + ONE});
    labelButton->AddClickCallback(clickCallBack);
    AddControl(labelButton);

    formatSettings.backgroundColor = BackgroundColor::Yellow;
}

bool TerminalRadioButton::SetEnable(bool isEnable) {
    bool isChanged = this->isEnable != isEnable;
    this->isEnable = isEnable;
    radioButton->SetText(this->isEnable ? Utf8String(EnableRune) : Utf8String(DisableRune) + " ");

    if (isChanged) {
        if (changedCallback != nullptr) {
            changedCallback(this);
        }
    }
    if (isEnable) {
        for (auto broControl : parent->GetControls()) {
            if (auto rb = broControl->As<TerminalRadioButton>(); rb != nullptr) {
                if (rb != this) {
                    rb->SetEnable(false);
                }
            }
        }
    }
    return isChanged;
    // TODO: disable all RadioButton for parent controls
}

bool TerminalRadioButton::GetEnable() {
    return isEnable;
}
