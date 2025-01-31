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


struct Header {
    FieldType type;

    bool operator == (const Header& other) const {
        return type == other.type;
    }
};

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


struct DataField {
    Header header;
    FieldValue val;

    static DataField Int(int value);
    static DataField String(std::string_view value);
    static DataField Double(double value);
    static DataField Date(storage::date value);

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

    bool operator == (const DataField& other) const {
        return std::tie(header, val) == std::tie(other.header, other.val);
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