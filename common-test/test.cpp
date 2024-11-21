#include "gtest/gtest.h"

#include "rune.h"

TEST(Runes, ConstructorEngTests) {
    Rune r("abc");
    Rune target('a');
    EXPECT_EQ(r, target);
}

TEST(Runes, ConstructorRusTests) {
    char s[] = "�";
    Rune r("����");
    EXPECT_EQ(2, utf8SymbolLen('�'));
    Rune target("�");
    EXPECT_EQ(r, target);
}