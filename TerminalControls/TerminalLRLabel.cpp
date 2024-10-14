#include "TerminalLRLabel.h"
#include "TerminalButton.h"
#include "ListDataProvider.h"

TerminalLRLabel::TerminalLRLabel(ListDataProviderPtr dataProvider, TerminalCoord position)
    : TerminalCompositeControl(position)
    , dataProvider(dataProvider)
{
    btnLeft = TerminalButton::Create("◀ ", TerminalCoord{ .row = 0, .col = 0 });
    label = TerminalLabel::Create(dataProvider, TerminalCoord{ .row = 0, .col = btnLeft->ColEnd() + ONE }, TextStyle::Underline);
    btnRight = TerminalButton::Create(" ▶", TerminalCoord{ .row = 0, .col = label->ColEnd() + ONE });

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

// https://www.utf8icons.com/character/9656/black-right-pointing-small-triangle
// https://www.utf8icons.com/character/9658/black-right-pointing-pointer
void TerminalLRLabel::CheckState() {
    Utf8String leftText = dataProvider->HasPrev() ? "◀ " : "◁ ";
    btnLeft->SetText(leftText);

    Utf8String rightText = dataProvider->HasNext() ? " ▶" : " ▷";
    btnRight->SetText(rightText);
}