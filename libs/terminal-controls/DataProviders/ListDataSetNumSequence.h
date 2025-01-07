#pragma once

#include "common.h"
#include "utf8string.h"
#include "DataProviders/ListDataSetBase.h"

#include <unordered_map>

class ListDataSetNumSequence : public ListDataSetBase {
public:
    DECLARE_CREATE(ListDataSetNumSequence);

    ListDataSetNumSequence(int seqBeg, int segEnd, int delta);
    const Utf8String& operator[] (size_t index) const override;
    bool IsValidIndex(size_t index) const override;
    bool Empty() const override;
    short MaxLen() const override;
    int GetPos(int value);
protected:
    int Value(size_t index) const;

protected:
    int segBeg;
    int segEnd;
    int delta = 0;
    mutable std::unordered_map<int, Utf8String> mem;
};

extern ListDataSetNumSequencePtr yearsDataSet;