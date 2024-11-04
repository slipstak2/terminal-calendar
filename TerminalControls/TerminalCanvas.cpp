#include "TerminalRootControl.h"
#include "TerminalCanvas.h"
#include "TerminalWindow.h"

TerminalCanvasPtr TerminalCanvas::Create(HANDLE outputHandle, short rows, short cols) {
    return std::make_unique<TerminalCanvas>(TerminalCanvas(outputHandle, rows, cols));
}

void TerminalCanvas::SetCursorPosition(short x, short y) {
    SetConsoleCursorPosition(outputHandle, {x, y});
}

const TerminalCell& TerminalCanvas::Get(short row, short col) const {
    return data[row][col];
}

TerminalCell& TerminalCanvas::Get(short row, short col) {
    return data[row][col];
}

void TerminalCanvas::Render(TerminalRootControlPtr rootControl) {
    rootControl->Flush();

    for (short col = 0; col < cols; ++col) {
        for (short row = 0; row < rows; ++row) {
            data[row][col].MakeSnapshot();
            if (data[row][col] != rootControl->data[row][col]) {
                SetCursorPosition(col, row);
                rootControl->data[row][col].Render();
                data[row][col] = rootControl->data[row][col];
            }
        }
    }
}