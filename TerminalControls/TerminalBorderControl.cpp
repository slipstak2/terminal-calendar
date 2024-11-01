#include "TerminalBorderControl.h"
#include "TerminalLabel.h"

TerminalBorderControl::TerminalBorderControl(const Utf8String& title, TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size){
    borderFormatSettings.fontColor = FontColor::Magenta;

    auto titleLabel = TerminalLabel::Create(title, TerminalCoord{.row = 0, .col = 3});
    titleLabel->SetFormatSettings(FormatSettings{.fontColor = FontColor::Yellow});
    TerminalCompositeControl::AddControl(titleLabel);

    containerControls = TerminalCompositeControl::Create(TerminalCoord{ .row = 1, .col = 1 }, TerminalSize{.height = Height() - 2, .width = Width() - 2});
    TerminalCompositeControl::AddControl(containerControls);
}

void TerminalBorderControl::AddControl(TerminalControlPtr control) {
    containerControls->AddControl(control);
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
            data[row][col] = BackgroundCell;
        }
        data[row][Width() - 1] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
    }
    FlushBottomBorder();
}