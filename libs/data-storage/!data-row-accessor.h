#pragma once

class DataRow;

class DataFieldAccessor { // aka DataRowWrapper
public:

    virtual size_t FieldsCount() = 0;
    virtual FieldData GetField(size_t field_num) = 0;
    virtual FieldData GetField(const std::string_view field_name) = 0;
    virtual std::string_view GetFieldName(size_t field_num) = 0;
    virtual DataRow GenRow() = 0;
};

class DataSetRow : public DataFieldAccessor {};
class DataStorageRow : public DataFieldAccessor {};
class DataViewRow : public DataFieldAccessor {};


using DataFieldAccessorPtr = std::shared_ptr<DataFieldAccessor>;


class DataRowAccessor { // for DataStorage/DataView/DataSet
    virtual size_t FieldsCount() const = 0;
    virtual size_t RowsCount() const = 0;
    virtual DataFieldAccessorPtr GetRow(size_t rowNum);
};
class FieldData;


