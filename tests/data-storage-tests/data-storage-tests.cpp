#include "pch.h"

#include "data-storage.h"
#include <tuple>


TEST(TestDataStorage, Empty) {
    using namespace exp1;

    DataStorage<std::string, int> ds;
    EXPECT_EQ(ds.Size(), 0);
}

TEST(TestDataStorage, Simple) {
    using namespace exp1;

    std::stringstream ss;
    DataStorage<std::string, int> ds;
    ds.Add(std::make_tuple<std::string, int>("Dan4ick", 28));
    ds.Add(std::make_tuple<std::string, int>("Misha", 25));
    ds.Add(std::make_tuple<std::string, int>("Igor", 38));

    EXPECT_EQ(ds.Size(), 3);

    ds.Print(ss, "");
    EXPECT_EQ(ss.str(), "(Dan4ick, 28)(Misha, 25)(Igor, 38)");
}
