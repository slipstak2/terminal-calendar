#include "TerminalGridCell.h"

TerminalGridCell::TerminalGridCell(Utf8String label, TerminalCoord position)
    : TerminalLabelFixedWidth(label, position)
{
    AddOnSelectedCallback([this](TerminalGridCell* sender, bool isSelected) {
        if (isSelected) {
            SetFontColor(FontColor::Blue);
            SetBackgroundColor(BackgroundColor::Brightcyan);
        } else {
            SetFontColor(FontColor::Default);
            SetBackgroundColor(BackgroundColor::Default);
        }
    });

    AddClickCallback([this]() {
        return SetSelected(!GetSelected());
    });
}

bool TerminalGridCell::SetSelected(bool isSelect) {
    bool isChanged = isSelected != isSelect;
    if (isChanged) {
        this->isSelected = isSelect;
        for (auto& selectedCallback : selectedCallbacks) {
            selectedCallback(this, isSelected);
        }
    }
    return isChanged;
}
bool TerminalGridCell::GetSelected() {
    return isSelected;

}