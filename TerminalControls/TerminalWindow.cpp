#include "TerminalWindow.h"

void TerminalWindow::AddControl(TerminalControlPtr control) {
    control->SetParentWindow(this);
    TerminalControl::AddControl(control);
}