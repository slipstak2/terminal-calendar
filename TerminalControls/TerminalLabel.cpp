#include "TerminalLabel.h"
#include "ListDataProvider.h"

TerminalLabel::TerminalLabel(const Utf8String& label, TerminalCoord position)
    : TerminalControl(position, { .height = 1, .width = (short)label.size() })
    , text(label)
{}

TerminalLabel::TerminalLabel(ListDataProviderPtr dataProvider, TerminalCoord position) 
    : TerminalControl(position, { .height = 1, .width = dataProvider->MaxLen()})
    , dataProvider(dataProvider)
{
    auto onChangeData = [](const Utf8String& prev, const Utf8String& current) {
        return true;
    };
    dataProvider->AddChangeCallback(onChangeData);
}

const Utf8String& TerminalLabel::Get() const {
    if (dataProvider && !dataProvider->Empty()) {
        return dataProvider->Get();
    }
    return text;
}
void TerminalLabel::FlushSelf() {
    const Utf8String& s = Get();
    short len = Size();
    Utf8String target(" ", len);
    short offset = (target.size() - s.size() + 1) / 2;
    for (int i = 0; i < s.size(); ++i) {
        target[i + offset] = s[i];
    }
    for (int i = 0; i < target.size(); ++i) {
        data[0][i] = CreateCell(target[i]);
    }
}

short TerminalLabel::Size() const {
    if (dataProvider && !dataProvider->Empty()) {
        return dataProvider->MaxLen();
    }
    else {
        return (short)text.size();
    }
}