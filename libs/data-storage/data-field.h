#pragma once

#include <chrono>
#include <string>
#include <string_view>

namespace storage {
    class date {
    public:
        date(int year, int month, int day) 
            : data(std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day))
        {}
        int year() {
            return static_cast<int>(data.year());
        }
        int month() {
            return static_cast<unsigned int>(data.month());
        }
        int day() {
            return static_cast<unsigned int>(data.day());
        }
        bool operator == (const date& other) const {
            return data == other.data;
        }
    private:
        std::chrono::year_month_day data;
    };

} // storage

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


union FieldValue {
    FieldValue() {
        Clear();
    }

    FieldValue(int value): Int(value) {
        Clear();
        Int = value;
    }
    FieldValue(std::string_view value) {
        Clear();
        String = value;
    }
    FieldValue(double value) {
        Clear();
        Double = value;
    }
    FieldValue(storage::date value) {
        Clear();
        Date = value;
    }

    inline void Clear() {
        memset(this, 0, sizeof(*this));;
    }

    int Int;
    std::string_view String;
    double Double;
    storage::date Date;
};

bool operator == (const FieldValue& lhs, const FieldValue& rhs);

template<typename T>
bool CheckTypeInternal(FieldType fieldType);

template<typename T>
bool CheckType(FieldType fieldType) {
    if (!CheckTypeInternal<T>(fieldType)) {
#if defined(_DEBUG)
        std::string message = "Bad cast: " + STRING(fieldType) + " -> " + STR<T>();
        throw std::runtime_error(message);
#else
        return false;
#endif
    }
    return true;
}


struct FieldData {
    FieldType type;
    FieldValue val;

    static FieldData Int(int value);
    static FieldData String(std::string_view value);
    static FieldData Double(double value);
    static FieldData Date(storage::date value);

    template<typename T>
    const T& Get() {
#if defined(_DEBUG)
        if (!CheckType<T>(type)) {
            std::string message = "Bad cast: " + STRING(type) + " -> " + STR<T>();
            throw std::runtime_error(message);
        }
#endif
        return *reinterpret_cast<T*>(&val);
    }

    bool operator == (const FieldData& other) const {
        return std::tie(type, val) == std::tie(other.type, other.val);
    }
};

struct FieldDesc {
    std::string name;
    FieldType type;
};

class CreateFieldDesc {
public:
    static FieldDesc Int(std::string name);
    static FieldDesc String(std::string name);
    static FieldDesc Double(std::string name);
    static FieldDesc Date(std::string name);
};