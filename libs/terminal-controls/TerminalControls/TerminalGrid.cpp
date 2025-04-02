#include "TerminalGrid.h"
#include "TerminalLabel.h"

TerminalGrid::TerminalGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position)
    : TerminalCompositeControl(position)
    , storage(storage) {

    short col = 0;
    bool isFirst = true;
    for (const Utf8String& h : header) {
        if (isFirst) {
            auto vertBorder = TerminalLabel::Create("|", TerminalCoord{.row = 0, .col = col++});
            AddControl(vertBorder);
            isFirst = false;
        }
        auto headerLabel = TerminalLabel::Create(h, TerminalCoord{ .row = 0, .col = col });
        headerLabel->AddClickCallback([headerLabel]() {
            auto formatSettings = headerLabel->GetFormatSettings();
            formatSettings.fontColor = formatSettings.fontColor != FontColor::Default ? FontColor::Default : FontColor::Yellow;
            headerLabel->SetFormatSettings(formatSettings);
            return true; 
            });
        col += (short)h.size();
        AddControl(headerLabel);

        auto vertBorder = TerminalLabel::Create("|", TerminalCoord{ .row = 0, .col = col++ });
        AddControl(vertBorder);
    }

    for (size_t row_num = 0; row_num < storage->RowsCount(); ++row_num) {
        col = 0;
        auto row = storage->GetRow(row_num);

        auto vertBorder = TerminalLabel::Create("|", TerminalCoord{ .row = 1 + (short)row_num, .col = col++ });
        AddControl(vertBorder);

        for (size_t field_num = 0; field_num < row->FieldsCount(); ++field_num) {
            std::string day(row->GetField<std::string_view>(field_num));
            auto dayLabel = TerminalLabel::Create(day, TerminalCoord{ .row = 1 + (short)row_num, .col = col });
            dayLabel->AddClickCallback([dayLabel]() {
                auto formatSettings = dayLabel->GetFormatSettings();
                formatSettings.fontColor = formatSettings.fontColor != FontColor::Default ? FontColor::Default : FontColor::Blue;
                formatSettings.backgroundColor = formatSettings.backgroundColor != BackgroundColor::Default ? BackgroundColor::Default : BackgroundColor::Brightcyan;
                dayLabel->SetFormatSettings(formatSettings);
                return true;
                });
            col += 2;
            AddControl(dayLabel);

            auto vertBorder = TerminalLabel::Create("|", TerminalCoord{ .row = 1 + (short)row_num, .col = col++ });
            AddControl(vertBorder);
        }

    }
    SetSize(TerminalSize{ .height = 1 + 6, .width = 2 * 7 + 8 });
}
