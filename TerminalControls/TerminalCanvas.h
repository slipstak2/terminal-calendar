#pragma once
#include "utils/common.h"
#include "TerminalCell.h"
#include "TerminalRectangle.h"
#include <Windows.h>

class TerminalCanvas {
public:
    static TerminalCanvasPtr Create(HANDLE outputHandle, short rows, short cols);
    const TerminalCell& Get(TerminalCoord position) const;
    TerminalCell& Get(TerminalCoord position);
    void Render(TerminalRootControlPtr rootControl);
private:
    void SetCursorPosition(short x, short y);
    TerminalCanvas(HANDLE outputHandle, short rows, short cols) : outputHandle(outputHandle), rows(rows), cols(cols)
    {
        data.resize(rows, std::vector<TerminalCell>(cols));
    }
private:
    HANDLE outputHandle;
    short rows;
    short cols;

    std::vector<std::vector<TerminalCell>> data;
};