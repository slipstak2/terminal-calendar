#include "TerminalCompositeControl.h"

TerminalCompositeControl::TerminalCompositeControl(TerminalCoord position)
    : TerminalControl(position)
{}

void TerminalCompositeControl::FlushSelf() {}

void TerminalCompositeControl::AddControl(TerminalControlPtr control) {
    control->SetParentWindow(parentWindow);
    TerminalControl::AddControl(control);
}