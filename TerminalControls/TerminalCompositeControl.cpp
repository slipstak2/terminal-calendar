#include "TerminalCompositeControl.h"

TerminalCompositeControl::TerminalCompositeControl(TerminalCoord position)
    : TerminalControl(position)
{}

TerminalCompositeControl::TerminalCompositeControl(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
{}

void TerminalCompositeControl::FlushSelf() {}

void TerminalCompositeControl::AddControl(TerminalControlPtr control) { //TODO: switch beetween windows
    control->SetParentWindow(parentWindow);
    TerminalControl::AddControl(control);
}