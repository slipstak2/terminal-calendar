#pragma once
#include "common.h"

class DataSet {
public:
    class DataRowAccessor {
    public:
        DataRowAccessor(size_t rowNum, DataSet* self) : rowNum(rowNum), dataSet(self)
        {}
        int get() {
            return 12;
        }
    protected:
        size_t rowNum;
        DataSet* dataSet;
    };
public:
    static DataSetPtr Create(DataViewPtr view) {
        return DataSetPtr(new DataSet(view));
    }

    size_t RowsCount() const {
        return view->RowsCount();
    }
    DataFieldAccessorPtr GetViewRow(size_t num) {
        return view->GetRow(num);
    }

    size_t FieldsCount() {
        return view->FieldsCount() + storage->FieldsCount();
    }

    template<typename AddColumnCb>
    void AddColumn(const FieldDesc& fd, const AddColumnCb& cb) {
        storage->AddFieldDesc(fd);
        assert(storage->Empty()); // fix it if add Second+ solum

        storage->AddEmptyRow();
        for (size_t num = 0; num < view->RowsCount(); ++num) {
           int value = cb(DataRowAccessor(num, this));
           DataRow& row = storage->AddEmptyRow();
           row.Fill(value);
           //storage->AddFieldData(num, FieldData::Int(value));
        }
    }

private:
    DataSet(DataViewPtr v): view(v), storage(DataStorage::Create()) {
    }

protected:
    DataViewPtr view;
    DataStoragePtr storage;
};