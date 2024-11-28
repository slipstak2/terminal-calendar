#include "TerminalLabelSwitcher.h"
#include "TerminalButton.h"
#include "DataProviders/ListDataProvider.h"
#include "TerminalLabelDataProvider.h"

// https://www.utf8icons.com/character/9656/black-right-pointing-small-triangle
// https://www.utf8icons.com/character/9658/black-right-pointing-pointer
const Utf8String TerminalLabelSwitcher::LeftActive    = "◀ ";
const Utf8String TerminalLabelSwitcher::LeftInactive  = "◁ ";
const Utf8String TerminalLabelSwitcher::RightActive   = " ▶";
const Utf8String TerminalLabelSwitcher::RightInactive = " ▷";

TerminalLabelSwitcher::TerminalLabelSwitcher(ListDataProviderPtr dataProvider, TerminalCoord position)
    : TerminalCompositeControl(position)
    , dataProvider(dataProvider)
{
    btnLeft = TerminalButton::Create(LeftActive, TerminalCoord{ .row = 0, .col = 0 });
    label = TerminalLabelDataProvider::Create(dataProvider, TerminalCoord{ .row = 0, .col = btnLeft->ColEnd() + ONE });
    btnRight = TerminalButton::Create(RightActive, TerminalCoord{ .row = 0, .col = label->ColEnd() + ONE });

    btnLeft->AddClickCallback([dataProvider, this]() {
        bool result = dataProvider->Prev();
        if (result) {
            CheckState();
        }
        return result;
        }
    );

    btnRight->AddClickCallback([dataProvider, this]() {
        bool result = dataProvider->Next();
        if (result) {
            CheckState();
        }
        return result;
        }
    );

    AddControl(btnLeft);
    AddControl(label);
    AddControl(btnRight);
    CheckState();

    SetSize({ .height = 1, .width = btnRight->ColEnd() + ONE });
}

void TerminalLabelSwitcher::CheckState() {
    btnLeft->SetText(dataProvider->HasPrev() ? LeftActive : LeftInactive);
    btnRight->SetText(dataProvider->HasNext() ? RightActive : RightInactive);
}

void TerminalLabelSwitcher::SetLabelFormatSettings(const FormatSettings labelFormatSettings) {
    label->SetFormatSettings(labelFormatSettings);
}