#pragma once

#include "common.h"
#include "data-storage.h"

class DataView {
public:
    DataView(DataStoragePtr s) : storage(s) {
        size_t cnt = storage->RowsCount();
        indexes.reserve(cnt);
        for (size_t idx = 0; idx < cnt; ++idx) {
            indexes[idx] = idx;
        }
    }

protected:
    DataStoragePtr storage;
    std::vector<size_t> indexes;
};