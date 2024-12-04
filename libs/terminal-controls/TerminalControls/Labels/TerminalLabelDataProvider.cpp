#include "TerminalLabelDataProvider.h"
#include "DataProviders/ListDataProvider.h"

TerminalLabelDataProvider::TerminalLabelDataProvider(ListDataProviderPtr dataProvider, TerminalCoord position)
    : TerminalLabelBase(position, { .height = 1, .width = dataProvider ? dataProvider->MaxLen(): 0 })
    , dataProvider(dataProvider)
{
    auto onChangeData = [](const Utf8String& prev, const Utf8String& current) {
        return true;
    };
    if (dataProvider) {
        dataProvider->AddChangeCallback(onChangeData);
    }
}

const Utf8String& TerminalLabelDataProvider::Get() const {
    if (dataProvider && !dataProvider->Empty()) {
        return dataProvider->Get();
    }
    return Utf8String::Empty;
}

void TerminalLabelDataProvider::FlushSelf() {
    FlushSelfAllignCenter(Get());
}

short TerminalLabelDataProvider::Length() const {
    if (dataProvider && !dataProvider->Empty()) {
        return dataProvider->MaxLen();
    }
    return 0;
}