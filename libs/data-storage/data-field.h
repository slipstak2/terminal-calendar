#pragma once

#include <chrono>
#include <string>
#include <tuple>
#include <string_view>
#include <deque>

namespace storage {
    class date {
    public:
        date();
        date(int year, int month, int day);
        date(std::chrono::year_month_day value);
        bool ok() const;
        int year() const;
        int month() const;
        int day() const;
        bool operator == (const date& other) const;
        bool operator < (const date& other) const;
        bool operator <= (const date& other) const;
        bool operator > (const date& other) const;
        bool operator >= (const date& other) const;
        date operator + (const std::chrono::year y) const;
        std::chrono::weekday weekday();
        bool is_today() const;
        static date now();
        std::string to_string() const;
        int distance_year(const date& prev) const;
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


class StringHeapStorage {
public:
    std::string_view Add(std::string_view sv) {
        data.emplace_back(sv);
        return data.back();
    }
protected:
    std::deque<std::string> data;
};


union FieldValue {
    static StringHeapStorage StringStorage;

    FieldValue() {
        Clear();
    }

    FieldValue(int value): Int(value) {
        Clear();
        Int = value;
    }
    FieldValue(std::string_view value) {
        Clear();
        String = StringStorage.Add(value);
    }
    FieldValue(std::string value) {
        Clear();
        String = StringStorage.Add(std::move(value));
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


struct FieldData {
    FieldType type;
    FieldValue val;

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
        val.String = FieldValue::StringStorage.Add(std::move(value));
    }

    template<>
    void Set(std::string_view value) {
        val.String = FieldValue::StringStorage.Add(value);
    }

    bool operator == (const FieldData& other) const {
        if (type != other.type) {
            return false;
        }
        if (type == FieldType::STRING) {
            return val.String == other.val.String;
        }
        if (type == FieldType::DOUBLE) {
            return fabs(val.Double - other.val.Double) <= EPS;
        }
        
        return val == other.val;
    }
public:
    static constexpr double EPS = 1e-8;
};

struct FieldDesc {
    std::string name;
    FieldType type;

    static FieldDesc Int(std::string name);
    static FieldDesc String(std::string name);
    static FieldDesc Double(std::string name);
    static FieldDesc Date(std::string name);
};

std::ostream& operator << (std::ostream& out, const FieldData& data);
