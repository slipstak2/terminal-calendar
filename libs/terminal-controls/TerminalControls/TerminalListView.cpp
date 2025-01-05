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
        return SetSelectedItem(relPosition.row + viewOffset);
        };

    for (short row = 0; row < Height(); ++row) {
        auto label = TerminalLabelFixedWidth::Create(TerminalCoord{ .row = row }, TerminalSize{.height = 1, .width = size.width});
        label->AddClickCallbackWithPosition([clickCallback](TerminalCoord relPosition, TerminalCoord absPosition) {
            return clickCallback(absPosition);
            });

        AddControl(label);
    }
    AddChangeOffsetCallback([this](const TerminalListView* listView, int curOffset, int prvOffset) {
        UpdateViewSelectedItem();
        });
    AddMouseWheelCallback([this](short wheelValue) {
        return ChangeOffset(wheelValue > 0 ? -3 : 3);
    });
    AddKeyPressUpOrDownCallbacks([this](bool isUp) {
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

bool TerminalListView::NeedScroll() {
    return Height() < TotalItems();
}

bool TerminalListView::HasUp() {
    return viewOffset > 0;
}

bool TerminalListView::HasDown() {
    return viewOffset < MaxViewOffset();
}

void TerminalListView::AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback) {
    changeItemsCountCallbacks.push_back(std::move(changeItemsCountCallback));
}

void TerminalListView::AddChangeOffsetCallback(TerminalListViewChangedOffsetCallback changeOffsetCallback) {
    changeOffsetCallbacks.push_back(std::move(changeOffsetCallback));
}

void TerminalListView::OnChangeItemsCount(size_t curItemsCount, size_t prvItemsCount) {
    for (auto& callback : changeItemsCountCallbacks) {
        callback(this, curItemsCount, prvItemsCount);
    }
}

void TerminalListView::OnChangeOffset(int curOffset, int prvOffset) {
    for (auto& callback : changeOffsetCallbacks) {
        callback(this, curOffset, prvOffset);
    }
}

int TerminalListView::TotalItems() const{
    return dataSet->TotalItems();
}

bool TerminalListView::ChangeOffset(int delta) {
    return SetOffset(viewOffset + delta);
}

int TerminalListView::GetOffset() const {
    return viewOffset;
}

bool TerminalListView::SetOffset(int newOffset) {
    int initViewOffset = viewOffset;
    viewOffset = NormalizeOffset(newOffset);
    if (viewOffset != initViewOffset) {
        OnChangeOffset(viewOffset, initViewOffset);
        return true;
    }
    return false;
}

int TerminalListView::MaxViewOffset() {
    if (TotalItems() >= Height()) {
        return TotalItems() - Height();
    }
    return 0;
}

int TerminalListView::NormalizeOffset(int offset) {
    offset = std::max(0, offset);
    offset = std::min(offset, MaxViewOffset());
    return offset;
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

bool TerminalListView::SetSelectedItem(int itemNum) {
    if (itemNum != -1) {
        if (itemNum < 0 || itemNum >= TotalItems()) {
            return false;
        }
    }
    if (selectedItem == itemNum) {
        return false;
    }
    selectedItem = itemNum;
    UpdateViewSelectedItem();
    return true;
}

int TerminalListView::GetSelectedItem() {
    return selectedItem;
}

bool TerminalListView::IsSelectedItemInView() {
    if (selectedItem == -1) {
        return false;
    }
    return viewOffset <= selectedItem && selectedItem < viewOffset + Height();
}
bool TerminalListView::NavigateOnSelectedItem() {
    if (selectedItem == -1) {
        return false;
    }
    SetOffset(selectedItem - Height() / 2);
    return false;
}

void TerminalListView::UpdateViewSelectedItem() {
    int viewSelectedItem = selectedItem - viewOffset;
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
        if (selectedItem != 0) {
            isChange |= SetSelectedItem(selectedItem - 1);
            if (!IsSelectedItemInView()) {
                isChange |= ChangeOffset(-1);
            }
        }
    }
    else {
        isChange |= SetSelectedItem(selectedItem + 1);
        if (!IsSelectedItemInView()) {
            isChange |= ChangeOffset(1);
        }
    }
    return isChange;
}