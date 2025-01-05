#pragma once

#include "common.h"
#include "utf8string.h"
#include "slice.h"
#include "DataProviders/ListDynamicDataSet.h"

#include <deque>
#include <functional>

class ListViewDataProvider {
public:

    ListViewDataProvider(ListDynamicDataSetPtr dataSet);
    slice<Utf8String> GetView(int offset, int length);

protected:
    ListDynamicDataSetPtr dataSet;
};