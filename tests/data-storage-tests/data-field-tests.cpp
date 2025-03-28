#include "pch.h"

#include "data-field.h"


TEST(TestDataField, Int) {
    FieldData field = FieldData::Int(42);
    FieldData fieldT = FieldData::Create(42);
    EXPECT_EQ(field, fieldT);

    EXPECT_EQ(field.type, FieldType::INT);
    EXPECT_EQ(field.Get<int>(), 42);

    field.Set<int>(43);
    EXPECT_EQ(field.Get<int>(), 43);
}

TEST(TestDataField, Double) {
    FieldData field = FieldData::Double(42.5);
    FieldData fieldT = FieldData::Create<double>(42.5);
    EXPECT_EQ(field, fieldT);

    EXPECT_EQ(field.type, FieldType::DOUBLE);
    EXPECT_EQ(field.Get<double>(), 42.5);

    field.Set<double>(50.1);
    EXPECT_EQ(field.Get<double>(), 50.1);
}

TEST(TestDataField, String) {
    FieldData field = FieldData::String("name");
    FieldData fieldT = FieldData::Create<std::string_view>("name");
    EXPECT_EQ(field, fieldT);

    EXPECT_EQ(field.type, FieldType::STRING);
    EXPECT_EQ(field.Get<std::string_view>(), "name");

    field.Set<std::string>("Name");
    EXPECT_EQ(field.Get<std::string_view>(), "Name");
}

TEST(TestDataField, Date) {
    storage::date ymd(2025, 1, 27);
    FieldData field = FieldData::Date(ymd);
    FieldData fieldT = FieldData::Create<storage::date>(ymd);
    EXPECT_EQ(field, fieldT);

    EXPECT_EQ(field.type, FieldType::DATE);
    EXPECT_EQ(field.Get<storage::date>(), ymd);
    
    storage::date new_ymd(2025, 2, 7);
    field.Set<storage::date>(new_ymd);
    EXPECT_EQ(field.Get<storage::date>(), new_ymd);
}

TEST(TestDataField, WrongCastDate) {
    storage::date ymd(2025, 1, 27);
    FieldData field = FieldData::Date(ymd);

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