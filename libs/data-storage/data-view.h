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

class DataView : public DataContainer, public std::enable_shared_from_this<DataView> {
    friend class DataViewRow;

public:
    using DataContainer::GetFieldIndex;

    OVERRIDE_DECLARATIONS_VIEW;
    
    DataContainerPtr AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) override;

    DataContainerPtr Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) override;

    DataContainerPtr Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) override;

    OVERRIDE_DECLARATIONS_UNIQUE;

    static DataViewPtr Create(DataContainerPtr container) {
        return DataViewPtr(new DataView(container));
    }
    
    static DataViewPtr Create(DataContainerPtr container, std::vector<size_t> fields_num, std::vector<size_t> rows_num) {
        return DataViewPtr(new DataView(container, std::move(fields_num), std::move(rows_num)));
    }

    template<typename ...FieldTypes>
    static DataViewPtr Create(DataContainerPtr container, FieldTypes... fields) {
        std::vector<size_t> fields_idx;
        fields_idx.reserve(sizeof...(fields));

        (..., (fields_idx.push_back(container->GetFieldIndex(fields))));

        return DataViewPtr(new DataView(container, fields_idx));
    }

    size_t RowsCount() const override;

    std::string_view GetFieldName(size_t field_num) const override;

    size_t GetFieldIndex(const std::string_view field_name) const override;

    DataFieldAccessorPtr GetRow(size_t row_num) override;
    DataFieldAccessorPtr GetRowFromParent(size_t row_num);

    size_t FieldsCount() const override;

private:
    DataView(DataContainerPtr container);

    DataView(DataContainerPtr container, std::vector<size_t> fields_num);

    DataView(DataContainerPtr container, std::vector<size_t> fields_num, std::vector<size_t> rows_num);

    void InitAllRows();
    void InitAllFields();
    void InitFieldsMapping();

protected:
    DataContainerPtr container;
    std::vector<size_t> rows_num;
    std::vector<size_t> fields_num;
    std::map<std::string_view, size_t> dv_fields_mapping;
};