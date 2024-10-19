#include "TerminalLabelDataProvider.h"
#include "ListDataProvider.h"


TerminalLabelDataProvider::TerminalLabelDataProvider(ListDataProviderPtr dataProvider, TerminalCoord position)
    : TerminalLabelBase(position, { .height = 1, .width = dataProvider->MaxLen() })
    , dataProvider(dataProvider)
{
    auto onChangeData = [](const Utf8String& prev, const Utf8String& current) {
        return true;
    };
    dataProvider->AddChangeCallback(onChangeData);
}

const Utf8String& TerminalLabelDataProvider::Get() const {
    if (dataProvider && !dataProvider->Empty()) {
        return dataProvider->Get();
    }
    return Utf8String::Empty;
}

short TerminalLabelDataProvider::Size() const {
    if (dataProvider && !dataProvider->Empty()) {
        return dataProvider->MaxLen();
    }
    return 0;
}