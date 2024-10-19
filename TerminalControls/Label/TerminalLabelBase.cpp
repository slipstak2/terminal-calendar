#include "TerminalLabelBase.h"

TerminalLabelBase::TerminalLabelBase(TerminalCoord position, TerminalSize size)
    : TerminalControl(position, size) {

}

void TerminalLabelBase::FlushSelf() {
    const Utf8String& s = Get();
    short len = Size();
    Utf8String target(" ", len);
    short offset = short(target.size() - s.size() + 1) / 2;
    for (int i = 0; i < s.size(); ++i) {
        target[i + offset] = s[i];
    }
    for (int i = 0; i < target.size(); ++i) {
        if (i < offset || i - offset >= s.size()) {
            data[0][i] = CreateBackgroundCell(target[i]);
        }
        else {
            data[0][i] = CreateCell(target[i]);
        }
    }
}