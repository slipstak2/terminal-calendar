#pragma once
#include "common.h"
#include "data-view.h"
#include "data-storage.h"
#include "data-field-accessor.h"

#include <functional>

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

class DataSet : public DataContainer, public std::enable_shared_from_this<DataSet> {
    friend class DataSetRow;
public:
    using DataContainer::GetFieldIndex;

    static DataSetPtr Create(DataViewPtr view) {
        return DataSetPtr(new DataSet(view));
    }

    OVERRIDE_DECLARATIONS_VIEW;

    DataContainerPtr SelfPtr() override;

    DataSetPtr AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) override;

    DataViewPtr Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) override;

    DataViewPtr Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) override;

    size_t RowsCount() const override;

    DataFieldAccessorPtr GetViewRow(size_t num) {
        return view->GetRow(num);
    }

    size_t FieldsCount() const override;

    size_t GetFieldIndex(const std::string_view field_name) const override;

    std::string_view GetFieldName(size_t field_num) const override;
    
    DataFieldAccessorPtr GetRow(size_t row_num) override;

private:
    DataSet(DataViewPtr v): view(v), storage(DataStorage::Create()) {
        storage->rows.resize(view->RowsCount());
    }

protected:
    DataViewPtr view;
    DataStoragePtr storage;
};