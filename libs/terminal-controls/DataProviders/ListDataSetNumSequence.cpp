#include "ListDataSetNumSequence.h"


ListDataSetNumSequence::ListDataSetNumSequence(int segBeg, int segEnd, int delta)
    : segBeg(segBeg)
    , segEnd(segEnd)
    , delta(delta)
{}

int ListDataSetNumSequence::Value(size_t index) const
{
    return segBeg + delta * (int)index;
}

const Utf8String& ListDataSetNumSequence::operator[] (size_t index) const {
    if (IsValidIndex(index)) {
        Utf8String& result = mem[(int)index];
        if (result.empty()) {
            result = std::to_string(Value(index));
        }
        return result;
    }
    return Utf8String::Empty;
}


bool ListDataSetNumSequence::IsValidIndex(size_t index) const {
    return Value(index) < segEnd;
}

bool ListDataSetNumSequence::Empty() const {
    return segBeg >= segEnd;
}

short ListDataSetNumSequence::MaxLen() const {
    return (short)std::max(
        std::to_string(segBeg).size(),
        std::to_string(segEnd).size()
    );
}

int ListDataSetNumSequence::GetPos(int value) {
    size_t index = (value - segBeg) / delta;
    if (IsValidIndex(index) && Value(index) == value) {
        return static_cast<int>(index);
    }
    return -1;
}



ListDataSetNumSequencePtr yearsDataSet = ListDataSetNumSequence::Create(1986, 9999, 1);