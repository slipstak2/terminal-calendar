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

    FieldType GetFieldType(int field_num) const {
        return GetFieldData(field_num).type;
    }

    template<typename T>
    T GetField(int field_num) const {
        FieldData data = GetFieldData(field_num);
        return data.Get<T>();
    }

    const FieldData& GetFieldData(size_t field_idx) const {
        return fields[field_idx];
    }

    template<typename T>
    void SetField(size_t field_num, T value) {
        CheckType<T>(fields[field_num].type);
        SetFieldInternal(field_num, value);
    }

    template<typename T>
    void SetFieldInternal(size_t field_num, T value) {
        memcpy(&fields[field_num].val, &value, sizeof(value));
    }

    template<>
    void SetFieldInternal<std::string>(size_t field_num, std::string value) {
        fields[field_num].val.String = stringStorage.Add(std::move(value));
    }

    template<>
    void SetFieldInternal<std::string_view>(size_t field_num, std::string_view value) {
        fields[field_num].val.String = stringStorage.Add(value);
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
