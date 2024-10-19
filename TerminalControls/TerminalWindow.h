#pragma once

#include "TerminalControl.h"

class TerminalWindow : public TerminalControl {
    //friend class TerminalCanvas;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::WINDOW)

    TerminalWindow(TerminalCoord lu, TerminalSize size)
        :TerminalControl(lu, size)
    {
        parentWindow = this;
    }
    void SetBackground(const TerminalCell& cell);
    void SetBorderColor(const FontColor& borderColor);
    void SetName(const std::string& newName);

    void AddControl(TerminalControlPtr control) override;

    static TerminalWindowPtr Create(TerminalCoord lu, TerminalSize size);

protected:
    void FlushSelf() override;
private:
    void FlushUpBorder();
    void FlushBottomBorder();

    TerminalCell BackgroundCell = CreateBackgroundCell(' ');
    FormatSettings borderFormatSettings = FormatSettings::Default;

    std::string name;
};

TerminalWindowPtr CreateBackgroundWindow(short rows, short cols);
TerminalWindowPtr CreateTerminalWindow(short rowBeg, short colBeg, short rowEnd, short colEnd);