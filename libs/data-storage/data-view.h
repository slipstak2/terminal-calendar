#pragma once

#include "data-storage.h"

class DataView {
public:
    DataView(DataStorage::Ptr s) : storage(s) {
        size_t cnt = storage->RowsCount();
        indexes.reserve(cnt);
        for (size_t idx = 0; idx < cnt; ++idx) {
            indexes[idx] = idx;
        }
    }

protected:
    std::shared_ptr<DataStorage> storage;
    std::vector<size_t> indexes;
};