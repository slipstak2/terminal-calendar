#pragma once
#include "common.h"
#include "TerminalCell.h"
#include "TerminalGeometry/TerminalRectangle.h"
#include <Windows.h>

class TerminalCanvas {
public:
    static TerminalCanvasPtr Create(HANDLE outputHandle, short rows, short cols);
    const TerminalCell& Get(TerminalCoord absPosition) const;
    TerminalCell& Get(TerminalCoord absPosition);
    void Render(TerminalRootControlPtr rootControl);
    void Resize(short rows, short cols);
    TerminalSize Size() const;
private:
    void SetCursorPosition(short x, short y);
    TerminalCanvas(HANDLE outputHandle, short rows, short cols);
private:
    HANDLE outputHandle;
    short rows;
    short cols;

    std::vector<std::vector<TerminalCell>> data;
};