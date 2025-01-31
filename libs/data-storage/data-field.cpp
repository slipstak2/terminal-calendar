#include "data-field.h"

std::string STRING(FieldType& fieldType) {
    if (fieldType == FieldType::INT) {
        return "INT";
    }
    if (fieldType == FieldType::STRING) {
        return "STRING";
    }
    if (fieldType == FieldType::DOUBLE) {
        return "DOUBLE";
    }
    if (fieldType == FieldType::DATE) {
        return "DATE";
    }
    return "UNKNOWN";
}

template<typename T>
std::string STR() {
    return "UNKNOWN";
}

template<>
std::string STR<int>() {
    return "INT";
}

template<>
std::string STR<std::string_view>() {
    return "STRING";
}

template<>
std::string STR<double>() {
    return "DOUBLE";
}

template<>
std::string STR<storage::date>() {
    return "DATE";
}

template<>
bool CheckTypeInternal<int>(FieldType fieldType) {
    return fieldType == FieldType::INT;
}

template<>
bool CheckTypeInternal<std::string_view>(FieldType fieldType) {
    return fieldType == FieldType::STRING;
}

template<>
bool CheckTypeInternal<double>(FieldType fieldType) {
    return fieldType == FieldType::DOUBLE;
}

template<>
bool CheckTypeInternal<storage::date>(FieldType fieldType) {
    return fieldType == FieldType::DATE;
}

DataField DataField::Int(int value) {
    return { FieldType::INT, FieldValue(value)};
}

DataField DataField::String(std::string_view value) {
    return { FieldType::STRING, FieldValue(value)};
}

DataField DataField::Double(double value) {
    return { FieldType::DOUBLE, FieldValue(value)};
}

DataField DataField::Date(storage::date value) {
    return { FieldType::DATE, FieldValue(value)};
}


FieldDesc CreateField::Int(std::string name) {
    return { std::move(name), FieldType::INT };
}

FieldDesc CreateField::String(std::string name) {
    return { std::move(name), FieldType::STRING };
}

FieldDesc CreateField::Double(std::string name) {
    return { std::move(name), FieldType::DOUBLE};
}

FieldDesc CreateField::Date(std::string name) {
    return { std::move(name), FieldType::DATE };
}


bool operator == (const FieldValue& lhs, const FieldValue& rhs) {
    return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}