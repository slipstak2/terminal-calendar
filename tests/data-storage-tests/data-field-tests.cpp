#include "pch.h"

#include "data-field.h"
#include <tuple>


TEST(TestDataField, Int) {
    DataField field = DataField::Int(42);

    EXPECT_EQ(field.header.type, FieldType::INT);
    EXPECT_EQ(field.Get<int>(), 42);
}

TEST(TestDataField, Double) {
    DataField field = DataField::Double(42.5);

    EXPECT_EQ(field.header.type, FieldType::DOUBLE);
    EXPECT_EQ(field.Get<double>(), 42.5);
}

TEST(TestDataField, Date) {
    std::chrono::year_month_day ymd = { std::chrono::year(2025) / std::chrono::month(1) / std::chrono::day(27) };
    DataField field = DataField::Date(ymd);

    EXPECT_EQ(field.header.type, FieldType::DATE);
    EXPECT_EQ(field.Get<std::chrono::year_month_day>(), ymd);
}

TEST(TestDataField, WrongCastDate) {
    std::chrono::year_month_day ymd = { std::chrono::year(2025) / std::chrono::month(1) / std::chrono::day(27) };
    DataField field = DataField::Date(ymd);

#if defined(_DEBUG)
    try {
        int value = field.Get<int>();
        FAIL() << "No exception";
    }
    catch (std::runtime_error& ex) {
        std::string message = ex.what();
        EXPECT_EQ(message, "Bad cast: DATE -> INT");
    }
#endif
}