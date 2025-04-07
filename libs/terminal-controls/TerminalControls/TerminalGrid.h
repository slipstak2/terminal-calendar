#pragma once

#include "TerminalCompositeControl.h"
#include "data-storage.h"

#include "BorderFormat/BorderFormat.h"

struct ColumnInfo {
    short width = 2;
};

class TerminalGrid : public TerminalCompositeControl {
public:
    DECLARE_KIND(TerminalCompositeControl, TerminalControl::Kind::GRID)

    DECLARE_CREATE(TerminalGrid)

    TerminalGrid(const std::vector<Utf8String>& header, DataStoragePtr storage, TerminalCoord position);
    void SetBorderVisible(bool isVisible);

    const DataStoragePtr GetStorage() const;

protected:
    void InitHeader();
    void InitData();

    void FlushRowBorder(short row);
    void FlushSelf() override;

protected:
    std::vector<Utf8String> header;
    DataStoragePtr storage;
    std::vector<ColumnInfo> columns;

protected:
    FormatSettings borderFormatSettings = FormatSettings::Default;
    BorderFormat borderFormat = BorderFormat::Default;

    FontColor selectedHeaderColor = FontColor::Yellow;
    FontColor noSelectedHeaderColor = FontColor::Brightblack;
};
