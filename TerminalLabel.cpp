#include "TerminalLabel.h"

TerminalLabel::TerminalLabel(const Utf8String& label, TerminalCoord position)
	: TerminalControl(position, { .height = 1, .width = (short)label.size() })
	, text(label)
{
	data.resize(label.size());
}

void TerminalLabel::FlushSelf() {
	for (int i = 0; i < text.size(); ++i) {
		data[0][i] = CreateCell(text[i]);
	}
}