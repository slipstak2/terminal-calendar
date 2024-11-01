#include "TerminalCompositeControl.h"
#include "BorderFormat.h"

class TerminalBorderControl : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::BORDER_CONTROL)

    DECLARE_CREATE(TerminalBorderControl)

    TerminalBorderControl(const Utf8String& title, TerminalCoord position, TerminalSize size);

    void AddControl(TerminalControlPtr control) override;

protected:
    void FlushSelf() override;

private:
    void FlushUpBorder();
    void FlushBottomBorder();

    TerminalCell BackgroundCell = CreateBackgroundCell(' ');
    FormatSettings borderFormatSettings = FormatSettings::Default;
    BorderFormat borderFormat = BorderFormat::Default;

protected:
    TerminalCompositeControlPtr containerControls;
};