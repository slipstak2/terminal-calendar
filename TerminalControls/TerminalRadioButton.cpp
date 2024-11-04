#include "TerminalRadioButton.h"
#include "utils/utf8string.h"

// https://cloford.com/resources/charcodes/utf-8_geometric.htm
// https://unicode.org/charts/PDF/U25A0.pdf
const Rune TerminalRadioButton::SelectedRune  = Rune("◉"); // Rune("▣");
const Rune TerminalRadioButton::UnselectedRune = Rune("◯"); // Rune("□");

TerminalRadioButton::TerminalRadioButton(const Utf8String& label, TerminalCoord position)
    : TerminalCompositeControl(position, TerminalSize{.height = 1, .width = (short)label.size() + 2})
{
    auto clickCallBack = [this]() {
        return SetSelected(true);
        };
    
    radioButton = TerminalButton::Create(Utf8String(UnselectedRune) + " ", TerminalCoord{.row = 0, .col = 0});
    radioButton->AddClickCallback(clickCallBack);
    AddControl(radioButton);
    
    labelButton = TerminalButton::Create(label, TerminalCoord{ .row = 0, .col = ONE + ONE});
    labelButton->AddClickCallback(clickCallBack);
    AddControl(labelButton);

    formatSettings.backgroundColor = BackgroundColor::Yellow;
}

bool TerminalRadioButton::SetSelected(bool isSelected) {
    bool isChanged = this->isSelected != isSelected;
    this->isSelected = isSelected;
    radioButton->SetText(this->isSelected ? Utf8String(SelectedRune) : Utf8String(UnselectedRune) + " ");

    if (isChanged) {
        if (changedCallback != nullptr) {
            changedCallback(this, isSelected);
        }
    }
    if (isSelected) {
        for (auto broControl : parent->GetControls()) {
            if (auto rb = broControl->As<TerminalRadioButton>(); rb != nullptr) {
                if (rb != this) {
                    rb->SetSelected(false);
                }
            }
        }
    }
    return isChanged;
}

bool TerminalRadioButton::GetSelected() {
    return isSelected;
}
