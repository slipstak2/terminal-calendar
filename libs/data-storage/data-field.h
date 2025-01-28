#pragma once

#include <chrono>
#include <string>
#include <string_view>

enum class FieldType {
    INT,
    DOUBLE,
    STRING,
    DATE
};

std::string STRING(FieldType& fieldType);

template<typename T>
std::string STR();


template<typename T>
bool CheckType(FieldType fieldType);


struct Header {
    FieldType type;
};

union FieldValue {
    int Int;
    std::string_view String;
    double Double;
    std::chrono::year_month_day Date;
};

struct DataField {
    Header header;
    FieldValue val;
    
    static DataField Int(int value);
    static DataField String(std::string_view value);
    static DataField Double(double value);
    static DataField Date(std::chrono::year_month_day value);

    template<typename T>
    const T& Get() {
#if defined(_DEBUG)
        if (!CheckType<T>(header.type)) {
            std::string message = "Bad cast: " + STRING(header.type) + " -> " + STR<T>();
            throw std::runtime_error(message);
        }
#endif
        return *reinterpret_cast<T*>(&val);
    }
};

struct FieldDesc {
    std::string name;
    FieldType type;
};

class CreateField {
public:
    static FieldDesc Int(std::string name);
    static FieldDesc String(std::string name);
    static FieldDesc Double(std::string name);
    static FieldDesc Date(std::string name);
};