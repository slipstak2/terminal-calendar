#include "TerminalWindow.h"

void TerminalWindow::AddControl(TerminalControlPtr control) {
    control->SetParentWindow(this);
    TerminalControl::AddControl(control);
}

bool TerminalWindow::IsDraggable() {
    return true;
}
bool TerminalWindow::TryDraggingStart(TerminalCoord absPosition) {
    return true;
}

bool TerminalWindow::TryDragging(TerminalCoord delta) {
    position += delta;
    return true;
}

bool TerminalWindow::TryDraggingStop() {
    return true;
}