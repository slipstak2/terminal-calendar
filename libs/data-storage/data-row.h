#pragma once
#include "common.h"
#include "data-field.h"

#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <deque>

class StringHeapStorage {
public:
    std::string_view Add(std::string_view sv) {
        data.emplace_back(sv);
        return data.back();
    }
protected:
    std::deque<std::string> data;
};


class DataRowBuilder;
class DataStorage;

class DataRow {
    friend class DataRowBuilder;
    friend class DataStorage;

public:
    DataRow() = default;
    DataRow(const std::initializer_list<FieldDesc>& fds) {
        fields.reserve(fds.size());
        for (const FieldDesc& fd : fds) {
            fields.push_back({ .type = fd.type });
        }
    }

    template<typename T>
    const T& GetField(size_t num) const;

    template<>
    const int& GetField<int>(size_t field_idx) const {
        // return fields[field_idx].Get<size_t>(); // TODO: Try this
        return fields[field_idx].val.Int;
    }

    template<>
    const std::string_view& GetField<std::string_view>(size_t field_idx) const {
        return fields[field_idx].val.String;
    }

    template<>
    const double& GetField<double>(size_t field_idx) const {
        return fields[field_idx].val.Double;
    }

    template<>
    const storage::date& GetField<storage::date>(size_t field_idx) const {
        return fields[field_idx].val.Date;
    }

    const FieldData& GetRawField(size_t field_idx) const {
        return fields[field_idx];
    }

    template<typename T>
    void SetField(size_t num, T value);

    template<>
    void SetField<int>(size_t num, int value) {
        CheckType<int>(fields[num].type);
        fields[num].val.Int = value;
    }

    template<>
    void SetField<std::string>(size_t num, std::string value) {
        CheckType<std::string_view>(fields[num].type);
        fields[num].val.String = stringStorage.Add(std::move(value));
    }

    template<>
    void SetField<std::string_view>(size_t num, std::string_view value) {
        CheckType<std::string_view>(fields[num].type);
        fields[num].val.String = stringStorage.Add(value);
    }

    template<>
    void SetField<double>(size_t num, double value) {
        CheckType<double>(fields[num].type);
        fields[num].val.Double = value;
    }

    template<>
    void SetField<storage::date>(size_t num, storage::date value) {
        CheckType<storage::date>(fields[num].type);
        fields[num].val.Date = value;
    }

    template<typename... Types>
    void Fill(Types... args) {
        FillImpl(0, args...);
    }

    size_t FieldsCount() const {
        return fields.size();
    }

    void ReserveFieldsCount(size_t fields_count) {
        fields.reserve(fields_count);
    }

    bool operator == (const DataRow& other) const {
        return fields == other.fields;
    }

private:
    template<typename T>
    void FillImpl(size_t num, T t) {
        SetField<T>(num, t);
    }

    template<typename T, typename ...Types>
    void FillImpl(size_t num, T t, Types... args) {
        FillImpl(num, t);
        FillImpl(num + 1, args...);
    }

    void AddFieldData(const FieldData& field) {
        fields.emplace_back(field);
    }

protected:
    std::vector<FieldData> fields;
    static StringHeapStorage stringStorage;
};

//
class DataRowBuilder {
public:
    DataRowBuilder() = default;
    DataRowBuilder(size_t fields_count) {
        result.ReserveFieldsCount(fields_count);
    }
    void Add(const FieldData& field) {
        result.AddFieldData(field);
    }
    const DataRow& Result() const {
        return result;
    }
protected:
    DataRow result;
};
