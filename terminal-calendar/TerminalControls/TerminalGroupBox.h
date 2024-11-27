#include "TerminalBorderControl.h"

class TerminalGroupBox : public TerminalBorderControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::GROUP_BOX)
    DECLARE_CREATE(TerminalGroupBox)

    TerminalGroupBox(const Utf8String& title, TerminalCoord position, TerminalSize size);
};