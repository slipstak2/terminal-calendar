#include "TerminalListView.h"
#include "TerminalLabelFixedWidth.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
    , provider(dataSet)
{
    dataSet->AddChangeItemsCountCallback([this](const ListDynamicDataSet* ds, size_t prvItemsCount) {
        OnChangeItemsCount(prvItemsCount);
    });

    auto clickCallback = [this](TerminalCoord absPosition) {
        TerminalCoord relPosition = GetRelativePosition(absPosition);
        return SetSelectedRow(relPosition.row + viewOffset);
    };

    for (short row = 0; row < Height(); ++row) {
        auto label = TerminalLabelFixedWidth::Create(TerminalCoord{ .row = row }, TerminalSize{.height = 1, .width = size.width});
        label->AddClickCallbackWithPosition([clickCallback](TerminalCoord relPosition, TerminalCoord absPosition) {
            return clickCallback(absPosition);
        });

        AddControl(label);
    }
    AddChangeOffsetCallback([this](const VerticalScrollableControl* listView, int prvOffset) {
        UpdateViewSelectedItem();
    });
    AddChangeSelectedRowCallback([this](const VerticalScrollableControl* listView, int prvSelectedRow) {
        UpdateViewSelectedItem(); 
    });

    AddMouseWheelCallback([this](short wheelValue) {
        return ChangeViewOffset(wheelValue > 0 ? -3 : 3);
    });
    AddKeyPressUpOrDownCallback([this](bool isUp) {
        return MoveSelectedRow(isUp);
    });
}

void TerminalListView::AddItem(const std::string& value) {
    bool incOffset = false;
    if (TotalItems() >= Height()) {
        if (viewOffset == MaxViewOffset()) {
            incOffset = true;
        }
    }
    dataSet->AddItem(value);
    if (incOffset) {
        ChangeViewOffset(1);
    }
}

bool TerminalListView::RemoveLastItem() {
    bool isRemove = dataSet->RemoveLastItem();
    ChangeViewOffset(0);
    return isRemove;
}

int TerminalListView::ViewItems() const {
    return Height();
}

int TerminalListView::TotalItems() const{
    return dataSet->TotalItems();
}

void TerminalListView::FlushSelf() {
    auto slice = provider.GetView(viewOffset, Height());
    for (size_t i = 0; i < slice.size(); ++i) {
        auto& text = slice[i];
        controls[i]->As<TerminalLabelFixedWidth>()->SetText(text);
    }
    for (size_t i = slice.size(); i < Height(); ++i) {
        controls[i]->As<TerminalLabelFixedWidth>()->SetText("");
    }
}

void TerminalListView::UpdateViewSelectedItem() {
    int viewSelectedItem = selectedRow - viewOffset;
    for (size_t row = 0; row < controls.size(); ++row) {
        controls[row]->SetFormatSettings(
            FormatSettings{ .textStyle = (row == viewSelectedItem ? TextStyle::Inverse : TextStyle::Default) });
    }
}
