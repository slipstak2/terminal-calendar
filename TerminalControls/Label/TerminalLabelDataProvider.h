#pragma once

#include "TerminalLabelBase.h"

class TerminalLabelDataProvider : public TerminalLabelBase {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LABEL_DATA_PROVIDER)

    DECLARE_CREATE(TerminalLabelDataProvider)

public:
    TerminalLabelDataProvider(ListDataProviderPtr dataProvider, TerminalCoord position);
    const Utf8String& Get() const override;
    short Length() const override;

protected:
    ListDataProviderPtr dataProvider = nullptr;
};