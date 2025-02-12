#pragma once

#include "common.h"
#include "data-storage.h"

class DataSetRow;

class DataViewRow : public DataFieldAccessor {
    friend class DataSetRow;
public:
    DataViewRow(const DataViewPtr view, size_t row_num);

public:
    size_t FieldsCount() const override;
    std::string_view GetFieldName(size_t field_num) const override;
    DataRow GetRow() const override;

protected:
    const FieldData& GetFieldData(size_t field_num)  const override;
    size_t GetFieldIndex(const std::string_view field_name) const override;

protected:
    DataViewPtr view;
    DataFieldAccessorPtr row;
};

class DataView : public DataContainer<DataView> {
    friend class DataViewRow;

public:
    using DataContainer<DataView>::GetFieldIndex;

    static DataViewPtr Create(DataStoragePtr s) {
        return DataViewPtr(new DataView(s));
    }
    template<typename ...FieldTypes>
    static DataViewPtr Create(DataStoragePtr s, FieldTypes... fields) {
        std::vector<size_t> fields_idx;
        fields_idx.reserve(sizeof...(fields));

        (..., (fields_idx.push_back(s->GetFieldIndex(fields))));

        return DataViewPtr(new DataView(s, fields_idx));
    }

    size_t RowsCount() const override;

    std::string_view GetFieldName(size_t field_num) const override;

    size_t GetFieldIndex(const std::string_view field_name) const override;

    DataFieldAccessorPtr GetRow(size_t row_num);
    DataFieldAccessorPtr GetRowFromParent(size_t row_num);

    size_t FieldsCount() const override;

private:
    DataView(DataStoragePtr s);

    DataView(DataStoragePtr s, std::vector<size_t> fields_num);

    void InitAllRows();
    void InitAllFields();
    void InitFieldsMapping();

protected:
    DataStoragePtr storage;
    std::vector<size_t> rows_idx;
    std::vector<size_t> fields_num;
    std::map<std::string_view, size_t> dv_fields_mapping;
};