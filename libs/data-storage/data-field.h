#pragma once

#include <chrono>

enum class FieldType {
    INT,
    DOUBLE,
    DATE
};

std::string STR(FieldType& fieldType) {
    if (fieldType == FieldType::INT) {
        return "INT";
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
bool CheckType<double>(FieldType fieldType) {
    return fieldType == FieldType::DOUBLE;
}

template<>
bool CheckType<std::chrono::year_month_day>(FieldType fieldType) {
    return fieldType == FieldType::DATE;
}


struct Header {
    FieldType type;
};

union FieldValue {
    int Int;
    double Double;
    std::chrono::year_month_day Date;
};

struct DataField {
    Header header;
    FieldValue val;

    
    static DataField Int(int value) {
        return { FieldType::INT, {.Int = value} };
    }
    static DataField Double(double value) {
        return { FieldType::DOUBLE, {.Double = value} };
    }

    static DataField Date(std::chrono::year_month_day value) {
        return { FieldType::DATE, {.Date = value} };
    }

    template<typename T>
    const T& Get() {
#if defined(_DEBUG)
        if (!CheckType<T>(header.type)) {
            std::string message = "Bad cast: " + STR(header.type) + " -> " + STR<T>();
            throw std::runtime_error(message);
        }
#endif
        return *reinterpret_cast<T*>(&val);
    }
};