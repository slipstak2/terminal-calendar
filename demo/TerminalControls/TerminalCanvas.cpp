#include "TerminalRootControl.h"
#include "TerminalCanvas.h"
#include "TerminalWindow.h"
#include "TerminalControlsConfig.h"

TerminalCanvasPtr TerminalCanvas::Create(HANDLE outputHandle, short rows, short cols) {
    return std::make_unique<TerminalCanvas>(TerminalCanvas(outputHandle, rows, cols));
}

TerminalCanvas::TerminalCanvas(HANDLE outputHandle, short rows, short cols)
    : outputHandle(outputHandle), rows(rows), cols(cols)
{
    data.resize(rows, std::vector<TerminalCell>(cols));
}

void TerminalCanvas::Resize(short newRows, short newCols) {
    short initRows = rows;
    short initCols = cols;
    data.resize(newRows, std::vector<TerminalCell>(newCols));
    for (int row = 0; row < data.size(); ++row) {
        data[row].resize(newCols);
    }
    rows = newRows;
    cols = newCols;
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
    TimeProfiler& tp = TControlsConfig().tp;
    tp.Push("Render");
    for (short col = 0; col < cols; ++col) {
        for (short row = 0; row < rows; ++row) {
            TerminalCell& prevFrameCell = data[row][col];
            TerminalCell& curFrameCell = rootControl->data[row][col];

            if (prevFrameCell.GetSnapshot() != curFrameCell || TControlsConfig().isFullRender) {
                renderCount++;
                SetCursorPosition(col, row);
                curFrameCell.Render();
            }
            prevFrameCell = curFrameCell;
            prevFrameCell.MakeSnapshot();
        }
    }
    tp.Pop("Render[" + std::to_string(renderCount) + "]");
}