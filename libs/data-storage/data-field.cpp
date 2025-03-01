#include "data-field.h"

StringHeapStorage FieldValue::StringStorage;

FieldType FieldTyper(int) {
    return FieldType::INT;
}

FieldType FieldTyper(double) {
    return FieldType::DOUBLE;
}

FieldType FieldTyper(std::string) {
    return FieldType::STRING;
}

FieldType FieldTyper(std::string_view) {
    return FieldType::STRING;
}

FieldType FieldTyper(storage::date) {
    return FieldType::DATE;
}

std::string ToString(const FieldType& fieldType) {
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

FieldData FieldData::Int(int value) {
    return { FieldType::INT, FieldValue(value)};
}

FieldData FieldData::String(std::string_view value) {
    return { FieldType::STRING, FieldValue(value) };
}

FieldData FieldData::Double(double value) {
    return { FieldType::DOUBLE, FieldValue(value) };
}

FieldData FieldData::Date(storage::date value) {
    return { FieldType::DATE, FieldValue(value) };
}

FieldDesc FieldDesc::Int(std::string name) {
    return { std::move(name), FieldType::INT };
}

FieldDesc FieldDesc::String(std::string name) {
    return { std::move(name), FieldType::STRING };
}

FieldDesc FieldDesc::Double(std::string name) {
    return { std::move(name), FieldType::DOUBLE};
}

FieldDesc FieldDesc::Date(std::string name) {
    return { std::move(name), FieldType::DATE };
}


bool operator == (const FieldValue& lhs, const FieldValue& rhs) {
    return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

std::ostream& operator << (std::ostream& out, const FieldData& data) {
    switch (data.type) {
    case FieldType::INT:
        out << data.val.Int;
        break;
    case FieldType::DOUBLE:
        out << data.val.Double;
        break;
    case FieldType::STRING:
        out << data.val.String;
        break;
    case FieldType::DATE:
        out << data.val.Date.day() << '.' << data.val.Date.month() << '.' << data.val.Date.year(); // TODO: add width for leading zero
        break;
    }
    return out;
}