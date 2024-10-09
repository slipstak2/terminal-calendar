#include "TerminalLabel.h"
#include "ListDataProvider.h"

TerminalLabel::TerminalLabel(const Utf8String& label, TerminalCoord position)
	: TerminalControl(position, { .height = 1, .width = (short)label.size() })
	, text(label)
{
	data.resize(label.size());
}

TerminalLabel::TerminalLabel(ListDataProviderPtr dataProvider, TerminalCoord position) 
	: TerminalControl(position, { .height = 1, .width = 20 })
	, dataProvider(dataProvider) {
}

void TerminalLabel::FlushSelf() {
	const Utf8String& s = dataProvider && !dataProvider->Empty() ? dataProvider->Get() : text;
	for (int i = 0; i < s.size(); ++i) {
		data[0][i] = CreateCell(s[i]);
	}
}