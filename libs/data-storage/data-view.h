#pragma once

#include "common.h"
#include "data-storage.h"

class DataViewRow : public DataFieldAccessor {
public:
    DataViewRow(const DataViewPtr view, size_t row_num);

public:
    size_t FieldsCount() const override;
    std::string_view GetFieldName(size_t field_num) const override;
    DataRow GetRow() override;

protected:
    const FieldData& GetFieldData(size_t field_num)  const override;
    size_t GetFieldIndex(const std::string_view field_name) const override;

protected:
    DataViewPtr view;
    const DataRow& row;
};

class DataView : public std::enable_shared_from_this<DataView> {
    friend class DataViewRow;

public:
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

    size_t RowsCount() const {
        return rows_idx.size();
    }

    std::string_view GetFieldName(size_t field_num) const {
        return storage->GetFieldName(fields_num[field_num]);
    }

    size_t GetFieldIndex(size_t field_num) const {
        return field_num;
    }

    size_t GetFieldIndex(const std::string_view field_name) const {
        return dv_fields_mapping.at(field_name);
    }

    DataFieldAccessorPtr GetRow(size_t row_num) {
        return std::make_shared<DataViewRow>(shared_from_this(), row_num);
    }

    size_t FieldsCount() const {
        return fields_num.size();
    }

private:
    DataView(DataStoragePtr s) : storage(s) {
        InitAllRows();
        InitAllFields();
        InitFieldsMapping();
    }

    DataView(DataStoragePtr s, std::vector<size_t> fields_num) : storage(s), fields_num(std::move(fields_num)) {
        InitAllRows();
        InitFieldsMapping();
    }

    void InitAllRows() {
        size_t rows_cnt = storage->RowsCount();
        rows_idx.resize(rows_cnt);
        for (size_t idx = 0; idx < rows_cnt; ++idx) {
            rows_idx[idx] = idx;
        }
    }
    void InitAllFields() {
        size_t fields_cnt = storage->FieldsCount();
        fields_num.resize(fields_cnt);
        for (size_t field_num = 0; field_num < fields_cnt; ++field_num) {
            fields_num[field_num] = field_num;
        }
    }
    void InitFieldsMapping() {
        for (size_t field_num = 0; field_num < fields_num.size(); ++field_num) {
            dv_fields_mapping[storage->GetFieldName(fields_num[field_num])] = field_num;
        }
    }
    const DataRow& GetRowFromStorage(size_t row_num);

protected:
    DataStoragePtr storage;
    std::vector<size_t> rows_idx;
    std::vector<size_t> fields_num; // TODO: rename on fields_num
    std::map<std::string_view, size_t> dv_fields_mapping;
};