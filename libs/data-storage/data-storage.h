#pragma once

#include "common.h"
#include "data-row.h"

#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <deque>
#include "data-field-accessor.h"


class DataStorageRow : public DataFieldAccessor {
public:
    DataStorageRow(const DataStoragePtr storage, size_t row_num);

    size_t FieldsCount() const override;
    std::string_view GetFieldName(size_t field_num) const override;
    DataRow GetRow() override;

protected:
    const FieldData& GetFieldData(size_t field_num)  const override;
    size_t GetFieldIndex(const std::string_view field_name) const override;

protected:
    DataStoragePtr storage;
    const DataRow& row;
};


class DataSet;

class DataStorage: public std::enable_shared_from_this<DataStorage> {
    friend class DataSet;
public:
    template<typename ...FieldDescT>
    static std::shared_ptr<DataStorage> Create(FieldDescT... fds) {
        return std::shared_ptr<DataStorage>(new DataStorage({fds...}));
    }

public:
    DataViewPtr View();

    template<typename ...FieldTypes>
    DataViewPtr View(FieldTypes... fields) {
        return DataView::Create(shared_from_this(), fields...);
    }

    size_t GetFieldIndex(size_t field_index) const;

    size_t GetFieldIndex(const std::string_view field_name) const;

    std::string_view GetFieldName(size_t field_num) const;

    const DataRow& GetDataRow(size_t row_num) const;

    DataFieldAccessorPtr GetRow(size_t row_num);

    size_t RowsCount() const;

    bool Empty() const;

    size_t FieldsCount() const;

    const FieldDesc& Field(size_t idx);

    DataRow& AddEmptyRow();

    template<typename... Types>
    DataRow& AddRow(Types... args) {
        DataRow& row = AddEmptyRow();
        row.Fill(args...);
        return row;
    }

private:
    explicit DataStorage(std::initializer_list<FieldDesc> fds) {
        row_dummy.ReserveFieldsCount(fds.size());
        ds_fields_desc.reserve(fds.size());

        for (const FieldDesc& fd : fds) {
            AddFieldDesc(fd);
        }
    }
    void AddFieldDesc(const FieldDesc& field_desc);
    
protected:
    DataRow row_dummy;
    std::vector<DataRow> rows;

    std::vector<FieldDesc> ds_fields_desc;
    std::map<std::string_view, size_t> ds_fields_mapping;
};
