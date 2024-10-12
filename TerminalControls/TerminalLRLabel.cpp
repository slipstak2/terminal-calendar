#include "TerminalLRLabel.h"
#include "TerminalButton.h"
#include "ListDataProvider.h"

TerminalLRLabel::TerminalLRLabel(ListDataProviderPtr dataProvider, TerminalCoord position)
    : TerminalCompositeControl(position)
{
    btnLeft = TerminalButton::Create("◀ ", TerminalCoord{ .row = 0, .col = 0 });
    label = TerminalLabel::Create(dataProvider, TerminalCoord{ .row = 0, .col = btnLeft->ColEnd() + ONE });
    btnRight = TerminalButton::Create(" ▶", TerminalCoord{ .row = 0, .col = label->ColEnd() + ONE });

    btnLeft->AddClickCallback([dataProvider, this]() {
        return dataProvider->Prev();
        }
    );

    btnRight->AddClickCallback([dataProvider, this]() {
        return dataProvider->Next();
        }
    );

    AddControl(btnLeft);
    AddControl(label);
    AddControl(btnRight);
    SetSize({ .height = 1, .width = btnRight->ColEnd() + ONE });
}