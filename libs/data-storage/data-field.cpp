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
std::string STR<std::chrono::year_month_day>() {
    return "DATE";
}

template<typename T>
bool CheckType(FieldType fieldType) {
    return false;
}

template<>
bool CheckType<int>(FieldType fieldType) {
    return fieldType == FieldType::INT;
}

template<>
bool CheckType<std::string_view>(FieldType fieldType) {
    return fieldType == FieldType::STRING;
}

template<>
bool CheckType<double>(FieldType fieldType) {
    return fieldType == FieldType::DOUBLE;
}

template<>
bool CheckType<std::chrono::year_month_day>(FieldType fieldType) {
    return fieldType == FieldType::DATE;
}

DataField DataField::Int(int value) {
    return { FieldType::INT, {.Int = value} };
}

DataField DataField::String(std::string_view value) {
    return { FieldType::STRING, {.String = value} };
}

DataField DataField::Double(double value) {
    return { FieldType::DOUBLE, {.Double = value} };
}

DataField DataField::Date(std::chrono::year_month_day value) {
    return { FieldType::DATE, {.Date = value} };
}

Field CreateField::String(std::string_view name) {
    return { name, DataField::String(name) };
}
