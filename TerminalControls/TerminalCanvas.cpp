#include "TerminalControlRoot.h"
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

void TerminalCanvas::Render(TerminalControlRootPtr rootControl) {
    rootControl->Flush();

    for (short col = 0; col < cols; ++col) {
        for (short row = 0; row < rows; ++row) {
            if (data[row][col] != rootControl->data[row][col]) {
                SetCursorPosition(col, row);
                rootControl->data[row][col].Render();
                data[row][col] = rootControl->data[row][col];
            }
        }
    }
}

void TerminalCanvas::AddWindow(TerminalWindowPtr window) {
    window->Flush();
    for (short row = window->RowBeg(); row <= window->RowEnd(); ++row) {
        for (short col = window->ColBeg(); col <= window->ColEnd(); ++col) {
            if (row < data.size() && col < data[row].size()) {
                data[row][col] = window->Get(row - window->RowBeg(), col - window->ColBeg());
            }
        }
    }
}