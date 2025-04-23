#define _CRT_SECURE_NO_WARNINGS

#include "data-field.h"
#include "utf8string.h"

namespace storage {

date::date() {
    memset(&data, 0, sizeof(data)); // TODO: default 01.01.1970 ?
} 
date::date(int year, int month, int day)
    : data(std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day))
{}
date::date(std::chrono::year_month_day value)
    : data(value)
{}
bool date::ok() const {
    return data.ok();
}
int date::year() const {
    return static_cast<int>(data.year());
}
int date::month() const {
    return static_cast<unsigned int>(data.month());
}
int date::day() const {
    return static_cast<unsigned int>(data.day());
}
bool date::operator == (const date& other) const {
    return data == other.data;
}
bool date::operator < (const date& other) const {
    return std::make_tuple(year(), month(), day()) < std::make_tuple(other.year(), other.month(), other.day());
}
bool date::operator <= (const date& other) const {
    return *this < other || *this == other;
}
bool date::operator > (const date& other) const {
    return other < *this;
}
bool date::operator >= (const date& other) const {
    return other <= *this;
}
date date::operator + (const std::chrono::year y) const {
    date result(year() + static_cast<int>(y), month(), day());
    return result;
}
std::chrono::weekday date::weekday() {
    return std::chrono::weekday(data);
}
bool date::is_today() const {
    return data == now();
}
date date::now() {

    using namespace std::chrono;

    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    return std::chrono::year_month_day(
        std::chrono::year(now->tm_year + 1900), 
        std::chrono::month(now->tm_mon + 1), 
        std::chrono::day(now->tm_mday)
    );
}

std::string date::to_string() const{
    static Utf8String mon[12] = {
        "Янв",
        "Фев",
        "Мар",
        "Апр",
        "Май",
        "Июн",
        "Июл",
        "Авг",
        "Сен",
        "Окт",
        "Ноя",
        "Дек",
    };
    char buf[256];
    sprintf(buf, "%02d-%s-%d", day(), mon[month() - 1].to_string().c_str(), year());
    return buf;
}

int date::distance_year(const date& prev) const {
    if (*this < prev) {
        return -prev.distance_year(*this);
    }
    int years = year() - prev.year();
    if (std::make_tuple(month(), day()) < std::make_tuple(prev.month(), prev.day())) {
        years--;
    }
    return years;
}

} // storage

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