#include "TerminalListView.h"
#include "TerminalLabelFixedWidth.h"

TerminalListView::TerminalListView(TerminalCoord position, TerminalSize size)
    : TerminalCompositeControl(position, size)
    , provider(dataSet)
{
    dataSet->AddChangeItemsCallback([this](const ListDynamicDataSet* ds, size_t curItemsCount, size_t prvItemsCount) {
        OnChangeItemsCount(curItemsCount, prvItemsCount);
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
        return ChangeOffset(wheelValue > 0 ? -3 : 3);
    });
    AddKeyPressUpOrDownCallback([this](bool isUp) {
        return MoveSelectedItem(isUp);
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
        ChangeOffset(1);
    }
}

bool TerminalListView::RemoveLastItem() {
    bool isRemove = dataSet->RemoveLastItem();
    ChangeOffset(0);
    return isRemove;
}

void TerminalListView::AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback) {
    changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
}

void TerminalListView::OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount) {
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, curItemsCount, prvItemsCount);
    }
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

bool TerminalListView::SetSelectedRow(int rowNum) {
    if (rowNum != -1) {
        if (rowNum < 0 || rowNum >= TotalItems()) {
            return false;
        }
    }
    if (selectedRow == rowNum) {
        return false;
    }
    int initSelectedRow = selectedRow;
    selectedRow = rowNum;
    OnChangeSelectedRow(initSelectedRow);
    return true;
}

int TerminalListView::GetSelectedItem() {
    return selectedRow;
}

bool TerminalListView::IsSelectedItemInView() {
    if (selectedRow == -1) {
        return false;
    }
    return viewOffset <= selectedRow && selectedRow < viewOffset + ViewItems();
}
bool TerminalListView::NavigateOnSelectedItem() {
    if (selectedRow == -1) {
        return false;
    }
    SetOffset(selectedRow - ViewItems() / 2);
    return false;
}

void TerminalListView::UpdateViewSelectedItem() {
    int viewSelectedItem = selectedRow - viewOffset;
    for (size_t row = 0; row < controls.size(); ++row) {
        controls[row]->SetFormatSettings(
            FormatSettings{ .textStyle = (row == viewSelectedItem ? TextStyle::Inverse : TextStyle::Default) });
    }
}

bool TerminalListView::MoveSelectedItem(bool isUp) {
    if (!IsSelectedItemInView()) {
        NavigateOnSelectedItem();
    }
    bool isChange = false;
    if (isUp) {
        if (selectedRow != 0) {
            isChange |= SetSelectedRow(selectedRow - 1);
            if (!IsSelectedItemInView()) {
                isChange |= ChangeOffset(-1);
            }
        }
    }
    else {
        isChange |= SetSelectedRow(selectedRow + 1);
        if (!IsSelectedItemInView()) {
            isChange |= ChangeOffset(1);
        }
    }
    return isChange;
}