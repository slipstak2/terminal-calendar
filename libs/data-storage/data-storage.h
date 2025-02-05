#pragma once

#include "common.h"
#include "data-row.h"

#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <deque>
#include "data-field-accessor.h"


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

    inline size_t GetFieldIndex(int field_index) const {
        return field_index;
    }

    inline size_t GetFieldIndex(const char* field_name) const {
        return ds_fields_mapping.at(field_name);
    }

    DataRow& AddEmptyRow() {
        rows.emplace_back(row_dummy);
        return rows.back();
    }

    template<typename... Types>
    DataRow& AddRow(Types... args) {
        DataRow& row = AddEmptyRow();
        row.Fill(args...);
        return row;
    }

    template<typename T>
    const T& GetField(DataRow& row, std::string_view field_name) { // TODO: where use???
        return row.GetField<T>(ds_fields_mapping[field_name]);
    }

    const DataRow& GetRow(size_t idx) const {
        return rows[idx];
    }

    DataRow GetRow(size_t idx, const std::vector<size_t>& fields_idx) {
        const DataRow& row = rows[idx];
        DataRowBuilder builder(fields_idx.size());
        for (size_t field_idx : fields_idx) {
            builder.Add(row.GetRawField(field_idx));
        }
        return builder.Result();
    }

    size_t RowsCount() const {
        return rows.size();
    }

    bool Empty() const {
        return rows.empty();
    }

    size_t FieldsCount() const {
        return ds_fields_desc.size();
    }

    const FieldDesc& Field(size_t idx) {
        return ds_fields_desc[idx];
    }

private:
    explicit DataStorage(std::initializer_list<FieldDesc> fds) {
        row_dummy.fields.reserve(fds.size());
        ds_fields_desc.reserve(fds.size());

        for (const FieldDesc& fd : fds) {
            AddFieldDesc(fd);
        }
    }
    void AddFieldDesc(const FieldDesc& field_desc);
    void AddFieldData(size_t row_idx, const FieldData& field_data);
    
protected:
    DataRow row_dummy;
    std::vector<DataRow> rows;

    std::vector<FieldDesc> ds_fields_desc;
    std::map<std::string_view, size_t> ds_fields_mapping;
};

class DataStorageRow : public DataFieldAccessor {
public:
    DataStorageRow(const DataStoragePtr storage, size_t row_idx)
        : storage(storage)
        , row(storage->GetRow(row_idx))
    {}

public:
    size_t FieldsCount() override;
    std::string_view GetFieldName(size_t field_num) override;
    DataRow GenRow() override;

protected:
    FieldData GetFieldData(int field_num) override;
    FieldData GetFieldData(const char* field_name) override;


protected:
    DataStoragePtr storage;
    const DataRow& row;
};