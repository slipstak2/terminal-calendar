#include "pch.h"

#include "data-field.h"


TEST(TestDataField, Int) {
    DataField field = DataField::Int(42);

    EXPECT_EQ(field.header.type, FieldType::INT);
    EXPECT_EQ(field.Get<int>(), 42);
}

TEST(TestDataField, String) {
    DataField field = DataField::String("name");

    EXPECT_EQ(field.header.type, FieldType::STRING);
    EXPECT_EQ(field.Get<std::string_view>(), "name");
}

TEST(TestDataField, Double) {
    DataField field = DataField::Double(42.5);

    EXPECT_EQ(field.header.type, FieldType::DOUBLE);
    EXPECT_EQ(field.Get<double>(), 42.5);
}

TEST(TestDataField, Date) {
    storage::date ymd(2025, 1, 27);
    DataField field = DataField::Date(ymd);

    EXPECT_EQ(field.header.type, FieldType::DATE);
    EXPECT_EQ(field.Get<storage::date>(), ymd);
}

TEST(TestDataField, WrongCastDate) {
    storage::date ymd(2025, 1, 27);
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