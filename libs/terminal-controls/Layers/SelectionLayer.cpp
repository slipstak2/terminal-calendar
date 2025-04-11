#include "Layers/SelectionLayer.h"

void SelectionLayer::AddControl(TerminalControl* control) {
    controls.push_back(control);
}

void SelectionLayer::Clear() {
    controls.clear();
}
