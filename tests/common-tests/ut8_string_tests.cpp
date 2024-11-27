#include "pch.h"

#include "utf8string.h"
#include <Windows.h>

class Utf8StringTests : public testing::Test
{
protected:
    Utf8StringTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~Utf8StringTests() override { }
};


TEST_F(Utf8StringTests, EmptyConstructor) {
    Utf8String s;
    EXPECT_EQ(0, s.size());
}

TEST_F(Utf8StringTests, ConstructorRusLetters) {
    Utf8String Igor("Игорь");
    EXPECT_EQ(5, Igor.size());
}

TEST_F(Utf8StringTests, ConstructorDiffLetters) {
    Utf8String capitals("Москва & 北京");
    EXPECT_EQ(11, capitals.size());

    std::vector<Rune> runes{ "М", "о", "с", "к", "в", "а", " ", "&", " ", "北", "京"};

    for (size_t idx = 0; idx < capitals.size(); ++idx) {
        EXPECT_EQ(capitals[idx], runes[idx]);
    }
}