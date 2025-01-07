#pragma once 
#include "TerminalCompositeControl.h"
#include "BorderFormat/BorderFormat.h"

class TerminalBorderControl : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::BORDER_CONTROL)

    TerminalBorderControl(const Utf8String& title, TerminalCoord position, TerminalSize size);

    void SetBorderColor(FontColor borderColor);
    void SetTitleColor(FontColor titleColor);
    void SetBorderFormat(BorderFormat borderFormat);
    void SetBorderVisible(bool isVisible);

    void AddControl(TerminalControlPtr control) override;
    void AddControlOnBorder(TerminalControlPtr control);

    std::vector<TerminalControlPtr>& GetControlsOnBorder();
    TerminalLabelPtr GetTitle() const;

protected:
    void FlushSelf() override;

protected:
    void FlushUpBorder();
    void FlushBottomBorder();

    FormatSettings borderFormatSettings = FormatSettings::Default;
    BorderFormat borderFormat = BorderFormat::Default;

protected:
    TerminalLabelPtr titleLabel;
    TerminalCompositeControlPtr containerControls;
};