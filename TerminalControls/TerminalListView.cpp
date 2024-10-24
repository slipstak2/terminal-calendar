#include "TerminalListView.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size)
{
    formatSettings.backgroundColor = BackgroundColor::Black;
    formatSettings.fontColor = FontColor::Yellow;
}

void TerminalListView::FlushSelf() {

    for (int row = 0; row < Height(); ++row) {
        for (int col = 0; col < Width(); ++col) {
            data[row][col] = CreateCell('M');
        }
    }

}