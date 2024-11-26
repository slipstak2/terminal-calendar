#include "pch.h"

#include "rune.h"

#include <Windows.h>

TEST(Runes, ConstructorEngTests) {
    Rune r("abc");
    Rune target('a');
    EXPECT_EQ(r, target);
}

TEST(Runes, ConstructorRusTests) {

    SetConsoleOutputCP(CP_UTF8);
    char s[] = "Б";
    Rune r("БВГД");
    Rune target("Б");
    EXPECT_EQ(r, target);
    EXPECT_EQ(2, r.size());
}