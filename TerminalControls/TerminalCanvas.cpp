#include "TerminalRootControl.h"
#include "TerminalCanvas.h"
#include "TerminalWindow.h"
#include "TerminalControlsConfig.h"

TerminalCanvasPtr TerminalCanvas::Create(HANDLE outputHandle, short rows, short cols) {
    return std::make_unique<TerminalCanvas>(TerminalCanvas(outputHandle, rows, cols));
}

void TerminalCanvas::SetCursorPosition(short x, short y) {
    SetConsoleCursorPosition(outputHandle, {x, y});
}

const TerminalCell& TerminalCanvas::Get(TerminalCoord absPosition) const {
    return data[absPosition.row][absPosition.col];
}

TerminalCell& TerminalCanvas::Get(TerminalCoord absPosition) {
    return data[absPosition.row][absPosition.col];
}

void TerminalCanvas::Render(TerminalRootControlPtr rootControl) {
    rootControl->Flush();

    int renderCount = 0;
    TControlsConfig().tp.Get();
    for (short col = 0; col < cols; ++col) {
        for (short row = 0; row < rows; ++row) {
            data[row][col].MakeSnapshot();
            //rootControl->data[row][col].MakeSnapshot(); // TODO: think comparation
            if (data[row][col] != rootControl->data[row][col]) {
                renderCount++;
                SetCursorPosition(col, row);
                rootControl->data[row][col].Render();
                data[row][col] = rootControl->data[row][col];
            }
        }
    }
    TControlsConfig().tp.Fix("   Render[" + std::to_string(renderCount) + "]: ");
}