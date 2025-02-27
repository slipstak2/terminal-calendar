#pragma once

#include "common.h"
#include "data-row.h"
#include "data-container.h"

#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <deque>
#include <numeric>
#include <unordered_set>


class DataStorageRow : public DataFieldAccessor {
public:
    DataStorageRow(const DataStoragePtr storage, size_t row_num);

    size_t FieldsCount() const override;
    std::string_view GetFieldName(size_t field_num) const override;
    DataRow GetRow() const override;

protected:
    const FieldData& GetFieldData(size_t field_num)  const override;
    size_t GetFieldIndex(const std::string_view field_name) const override;

protected:
    DataStoragePtr storage;
    const DataRow& row;
};


class DataSet;

class DataStorage: public DataContainer, public std::enable_shared_from_this<DataStorage> {
    friend class DataSet;

public:
    using DataContainer::GetFieldIndex;

    template<typename ...FieldDescT>
    static std::shared_ptr<DataStorage> Create(FieldDescT... fds) {
        return std::shared_ptr<DataStorage>(new DataStorage({fds...}));
    }

public:
    OVERRIDE_DECLARATIONS_VIEW;
    
    DataContainerPtr AddColumn(const FieldDesc& fd, const std::function<FieldValue(const DataFieldAccessor& row)>& add_column_cb) override;

    DataContainerPtr Select(const std::function<bool(const DataFieldAccessor& row)>& select_cb) override;

    DataContainerPtr Sort(const std::function<bool(const DataFieldAccessor& lsh, const DataFieldAccessor& rhs)>& cmp_cb) override;

    size_t GetFieldIndex(const std::string_view field_name) const override;

    std::string_view GetFieldName(size_t field_num) const override;

    const DataRow& GetDataRow(size_t row_num) const;

    DataFieldAccessorPtr GetRow(size_t row_num) override;

    size_t RowsCount() const override;

    bool Empty() const;

    size_t FieldsCount() const override;

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
        //ds_fields_desc.reserve(fds.size()); // std::deque don't have reserver method

        for (const FieldDesc& fd : fds) {
            AddFieldDesc(fd);
        }
    }
    void AddFieldDesc(const FieldDesc& field_desc);
    
protected:
    DataRow row_dummy;
    std::vector<DataRow> rows;

    std::deque<FieldDesc> ds_fields_desc;
    std::map<std::string_view, size_t> ds_fields_mapping;
};
