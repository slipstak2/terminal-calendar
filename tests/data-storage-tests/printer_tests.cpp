#include "pch.h"

#include "printer.h"

TEST(TestPrintTuplePretty, Simple) {

    std::stringstream ss;
    auto Dan4ick = std::make_tuple<std::string, int>("Dan4ick", 28);
    print_tuple_pretty(Dan4ick, ss);
    auto s = ss.str();
    EXPECT_EQ(ss.str(), "(Dan4ick, 28)");
}

TEST(TestPrintTuple, Simple) {

    std::stringstream ss;
    auto Misha = std::make_tuple<std::string, int>("Misha", 25);
    print_tuple(Misha, ss);
    auto s = ss.str();
    EXPECT_EQ(ss.str(), "Misha 25 ");
}