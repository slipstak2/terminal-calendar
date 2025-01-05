#pragma once

#include "common.h"
#include "utf8string.h"
#include "ListDataSet.h"
#include <functional>

using ChangeCallback = std::function<bool(const Utf8String& prev, const Utf8String& current)>;

class ListDataProvider {
public:
    DECLARE_CREATE(ListDataProvider)

    ListDataProvider(ListDataSetPtr dataSet);
    ListDataProvider(ListDataSetPtr dataSet, int pos);
    const Utf8String& Get() const;
    bool Next();
    bool HasNext();
    bool Prev();
    bool HasPrev();
    bool Empty() const;
    short MaxLen() const;

    void AddChangeCallback(ChangeCallback changeCallback);
protected:
    ListDataSetPtr dataSet;
    int pos = -1;
protected:
    void OnChange(const Utf8String& prev, const Utf8String& current);
    
    std::vector<ChangeCallback> changeCallbacks;
};
