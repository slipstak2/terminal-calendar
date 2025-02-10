#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <deque>

namespace storage {
    class date {
    public:
        date() = default; // TODO: default 01.01.1970 ?
        date(int year, int month, int day) 
            : data(std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day))
        {}
        int year() const {
            return static_cast<int>(data.year());
        }
        int month() const {
            return static_cast<unsigned int>(data.month());
        }
        int day() const {
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

FieldType FieldTyper(int);
FieldType FieldTyper(double);
FieldType FieldTyper(std::string);
FieldType FieldTyper(std::string_view);
FieldType FieldTyper(storage::date);

std::string ToString(const FieldType& fieldType);

template<typename T>
std::string ToString() {
    return ToString(FieldTyper(T()));
}

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
bool CheckType(FieldType fieldType) {
    if (FieldTyper(T()) != fieldType) {
#if defined(_DEBUG)
        std::string message = "Bad cast: " + ToString(fieldType) + " -> " + ToString<T>();
        throw std::runtime_error(message);
#else
        return false;
#endif
    }
    return true;
}

class StringHeapStorage {
public:
    std::string_view Add(std::string_view sv) {
        data.emplace_back(sv);
        return data.back();
    }
protected:
    std::deque<std::string> data;
};


struct FieldData {
    FieldType type;
    FieldValue val;

    static StringHeapStorage stringStorage;

    FieldData() = default;

    FieldData(const FieldType type, const FieldValue val) 
        : type(type), val(val)
    {}

    FieldData(const FieldType type) 
        : type(type)
    {}

    template<typename T>
    static FieldData Create(T value) {
        return FieldData(FieldTyper(value), FieldValue(value));
    }

    static FieldData Int(int value);
    static FieldData String(std::string_view value);
    static FieldData Double(double value);
    static FieldData Date(storage::date value);

    template<typename T>
    const T& Get() const {
#if defined(_DEBUG)
        CheckType<T>(type);
#endif
        return *reinterpret_cast<const T*>(&val);
    }

    template<typename T>
    void Set(T value) {
        memcpy(&val, &value, sizeof(value));
    }

    template<>
    void Set(std::string value) {
        val.String = stringStorage.Add(std::move(value));
    }

    template<>
    void Set(std::string_view value) {
        val.String = stringStorage.Add(value);
    }

    bool operator == (const FieldData& other) const {
        if (type != other.type) {
            return false;
        }
        if (type == FieldType::STRING) {
            return val.String == other.val.String;
        }
        
        return val == other.val;
    }
};

struct FieldDesc {
    std::string name;
    FieldType type;

    static FieldDesc Int(std::string name);
    static FieldDesc String(std::string name);
    static FieldDesc Double(std::string name);
    static FieldDesc Date(std::string name);
};
