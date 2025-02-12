#pragma once
#include "common.h"
#include "data-view.h"
#include "data-storage.h"
#include "data-field-accessor.h"

class DataSetRow : public DataFieldAccessor {
public:
    DataSetRow(const DataSetPtr dataSet, const size_t row_num);

    size_t FieldsCount() const override;
    std::string_view GetFieldName(size_t field_num) const override;
    DataRow GetRow() const override;

protected:
    const FieldData& GetFieldData(size_t field_num)  const override;
    size_t GetFieldIndex(const std::string_view field_name) const override;

protected:
    DataSetPtr dataSet;
    DataFieldAccessorPtr viewRow;
    DataFieldAccessorPtr storageRow;
};

class DataSet : public DataContainer<DataSet> {
    friend class DataSetRow;
public:
    using DataContainer<DataSet>::GetFieldIndex;

    static DataSetPtr Create(DataViewPtr view) {
        return DataSetPtr(new DataSet(view));
    }

    size_t RowsCount() const override;

    DataFieldAccessorPtr GetViewRow(size_t num) {
        return view->GetRow(num);
    }

    size_t FieldsCount() const override;

    size_t GetFieldIndex(const std::string_view field_name) const override;

    std::string_view GetFieldName(size_t field_num) const override;
    
    DataFieldAccessorPtr GetRow(size_t row_num);

    template<typename AddColumnCb>
    void AddColumn(const FieldDesc& fd, const AddColumnCb& cb) {
        storage->AddFieldDesc(fd);
        for (size_t row_num = 0; row_num < view->RowsCount(); ++row_num) {
            DataRow& storage_row = storage->rows[row_num];
            DataSetRow dataset_row(shared_from_this(), row_num);
            storage_row.AddFieldData(FieldData(fd.type, FieldValue(
                cb(dataset_row)
            )));
        }
    }

private:
    DataSet(DataViewPtr v): view(v), storage(DataStorage::Create()) {
        storage->rows.resize(view->RowsCount());
    }

protected:
    DataViewPtr view;
    DataStoragePtr storage;
};