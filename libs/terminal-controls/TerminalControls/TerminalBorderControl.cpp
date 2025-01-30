#include "TerminalBorderControl.h"
#include "TerminalLabel.h"

TerminalBorderControl::TerminalBorderControl(const Utf8String& title, TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size){

    titleLabel = TerminalLabel::Create(title, TerminalCoord{.row = 0, .col = 3});
    AddControlOnBorder(titleLabel);

    containerControls = TerminalCompositeControl::Create(TerminalCoord{ .row = 1, .col = 1 }, TerminalSize{.height = Height() - 2, .width = Width() - 2});
    AddControlOnBorder(containerControls);
}

void TerminalBorderControl::AddControl(TerminalControlPtr control) {
    containerControls->AddControl(control);
}

void TerminalBorderControl::AddControlOnBorder(TerminalControlPtr control) {
    TerminalCompositeControl::AddControl(control);
}

size_t TerminalBorderControl::RemoveControlsOnBorder(const FilterControlCallback& filterCallback) {
    return RemoveControls(TerminalCompositeControl::GetControls(), filterCallback);
}

TerminalLabelPtr TerminalBorderControl::GetTitle() const {
    return titleLabel;
}

void TerminalBorderControl::SetBorderColor(FontColor borderColor) {
    borderFormatSettings.fontColor = borderColor;
}

void TerminalBorderControl::SetTitleColor(FontColor titleColor) {
    titleLabel->SetFormatSettings(FormatSettings{ .fontColor = titleColor });
}

void TerminalBorderControl::SetBorderFormat(BorderFormat borderFormat) {
    this->borderFormat = borderFormat;
}

void TerminalBorderControl::SetBorderVisible(bool isVisible) {
    borderFormatSettings.textStyle = isVisible ? TextStyle::Default : TextStyle::Сonceal;
}

void TerminalBorderControl::FlushUpBorder() {
    short row = 0;
    data[row][0] = CreateCell(borderFormat.UpLeft(), &borderFormatSettings);
    for (short col = 1; col < Width() - 1; ++col) {
        data[row][col] = CreateCell(borderFormat.Horizontal(), &borderFormatSettings);
    }
    data[row][Width() - 1] = CreateCell(borderFormat.UpRight(), &borderFormatSettings);
}

void TerminalBorderControl::FlushBottomBorder() {
    short row = Height() - 1;
    data[row][0] = CreateCell(borderFormat.BottomLeft(), &borderFormatSettings);
    for (short col = 1; col < Width() - 1; ++col) {
        data[row][col] = CreateCell(borderFormat.Horizontal(), &borderFormatSettings);
    }
    data[row][Width() - 1] = CreateCell(borderFormat.BottomRight(), &borderFormatSettings);
}

void TerminalBorderControl::FlushSelf() {
    FlushUpBorder();
    for (short row = 1; row < Height() - 1; ++row) {
        data[row][0] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
        for (short col = 1; col < Width() - 1; ++col) {
            data[row][col] = backgroundCell;
        }
        data[row][Width() - 1] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
    }
    FlushBottomBorder();
}