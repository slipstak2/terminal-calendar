#pragma once

#include "common.h"
#include "data-storage.h"

class DataView {
public:
    static std::shared_ptr<DataView> Create(DataStoragePtr s) {
        return std::shared_ptr<DataView>(new DataView(s));
    }

    size_t RowsCount() const {
        return storage->RowsCount();
    }

    DataRow GetRow(size_t num) {
        DataRow row = storage->GetRow(rows_idx[num], fields_idx);
        return row;
    }

private:
    DataView(DataStoragePtr s) : storage(s) {
        size_t rows_cnt = storage->RowsCount();
        rows_idx.resize(rows_cnt);
        for (size_t idx = 0; idx < rows_cnt; ++idx) {
            rows_idx[idx] = idx;
        }

        size_t fields_cnt = storage->FieldsCount();
        fields_idx.resize(fields_cnt);
        for (size_t idx = 0; idx < fields_cnt; ++idx) {
            fields_idx[idx] = idx;
        }
    }

protected:
    DataStoragePtr storage;
    std::vector<size_t> rows_idx;
    std::vector<size_t> fields_idx;;
};