#include "pch.h"

#include "rune.h"
#include <Windows.h>

class RuneTests : public testing::Test
{
protected:
    RuneTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~RuneTests() override { }
};

TEST_F(RuneTests, ConstructorLatLetters) {
    Rune r("abc");
    Rune target('a');
    EXPECT_EQ(1, r.size());
    EXPECT_EQ(r, target);
}

TEST_F(RuneTests, ConstructorRusLetters) {
    Rune r("БВГД");
    Rune target("Б");
    EXPECT_EQ(r, target);
    EXPECT_EQ(2, r.size());
}

TEST_F(RuneTests, ConstructorFlag) {
    Rune r("Ⲗ");
    EXPECT_EQ(3, r.size());
}

TEST_F(RuneTests, ConstructorEmoji) {
    Rune r("🦉");
    EXPECT_EQ(4, r.size());
}
