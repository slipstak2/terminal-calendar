#include "ListViewDataProvider.h"

ListViewDataProvider::ListViewDataProvider(ListDynamicDataSetPtr dataSet)
    :dataSet(dataSet)
{}

slice<Utf8String> ListViewDataProvider::GetView(int offset, int length) {
    return dataSet->GetView(offset, length);
}