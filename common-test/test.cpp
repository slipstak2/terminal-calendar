#include "gtest/gtest.h"

#include "rune.h"

TEST(Runes, ConstructorEngTests) {
    Rune r("abc");
    Rune target('a');
    EXPECT_EQ(r, target);
}

TEST(Runes, ConstructorRusTests) {
    char s[] = "Á";
    Rune r("ÁÂÃÄ");
    EXPECT_EQ(2, utf8SymbolLen('Á'));
    Rune target("Á");
    EXPECT_EQ(r, target);
}