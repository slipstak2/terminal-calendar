#include "TerminalGrid.h"
#include "TerminalLabel.h"

TerminalGrid::TerminalGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position)
    : TerminalCompositeControl(position)
    , header(header)
    , storage(storage) {

    columns.resize(header.size());

    InitHeader();
    InitData();

    SetBorderVisible(false);
    SetSize(TerminalSize{ .height = ONE + (short)storage->RowsCount(), .width = 2 * 7 + 8});
}

void TerminalGrid::SetBorderVisible(bool isVisible) {
    borderFormatSettings.textStyle = isVisible ? TextStyle::Default : TextStyle::Conceal;
}

void TerminalGrid::InitHeader() {
    short col = 1;
    for (short column = 0; column < columns.size(); ++column) {
        auto headerLabel = TerminalLabel::Create(header[column], TerminalCoord{.row = 0, .col = col});
        headerLabel->AddClickCallback([headerLabel]() {
            auto formatSettings = headerLabel->GetFormatSettings();
            formatSettings.fontColor = formatSettings.fontColor != FontColor::Default ? FontColor::Default : FontColor::Yellow;
            headerLabel->SetFormatSettings(formatSettings);
            return true;
        });
        AddControl(headerLabel);
        col += columns[column].width + 1;
    }
}

void TerminalGrid::InitData() {
    for (size_t row_num = 0; row_num < storage->RowsCount(); ++row_num) {
        short col = 1;
        auto row = storage->GetRow(row_num);

        for (size_t field_num = 0; field_num < row->FieldsCount(); ++field_num) {
            std::string day(row->GetField<std::string_view>(field_num));
            if (!day.empty()) {
                auto dayLabel = TerminalLabel::Create(day, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                dayLabel->AddClickCallback([dayLabel]() {
                    auto formatSettings = dayLabel->GetFormatSettings();
                    formatSettings.fontColor = formatSettings.fontColor != FontColor::Default ? FontColor::Default : FontColor::Blue;
                    formatSettings.backgroundColor = formatSettings.backgroundColor != BackgroundColor::Default ? BackgroundColor::Default : BackgroundColor::Brightcyan;
                    dayLabel->SetFormatSettings(formatSettings);
                    return true;
                    });
                AddControl(dayLabel);
            }
            else {
                std::string empty_cell(columns[field_num].width, ' ');
                auto emptyLabel = TerminalLabel::Create(empty_cell, TerminalCoord{ .row = ONE + (short)row_num, .col = col });
                AddControl(emptyLabel);
            }
            col += columns[field_num].width + 1;

        }
    }
}

void TerminalGrid::FlushRowBorder(short row) {
    short col = 0;

    data[row][col] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
    for (short column = 0; column < columns.size(); ++column) {
        col += columns[column].width + 1;
        data[row][col] = CreateCell(borderFormat.Vertical(), &borderFormatSettings);
    }
}

void TerminalGrid::FlushSelf() {
    FlushRowBorder(0);
    for (short row_num = 0; row_num < storage->RowsCount(); ++row_num) {
        FlushRowBorder(ONE + row_num);
    }
}
