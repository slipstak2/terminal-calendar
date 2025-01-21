#include "pch.h"

#include "data-storage.h"
#include <tuple>


TEST(TestDataView, Empty) {
    using namespace exp1;

    DataStorage<std::string, int, char> ds;
    DataView<decltype(ds), 2, 0> dv(ds);
    EXPECT_EQ(dv.Size(), 0);
}


TEST(TestDataView, Simple) {
    using namespace exp1;

    std::stringstream ss;
    DataStorage<std::string, int, char> ds;
    ds.Add(std::make_tuple<std::string, int, char>("Dan4ick", 28, 'M'));
    ds.Add(std::make_tuple<std::string, int, char>("Misha", 25, 'M'));
    ds.Add(std::make_tuple<std::string, int, char>("Masha", 38, 'F'));

    DataView<decltype(ds), 2, 0> dv(ds);
    EXPECT_EQ(dv.Size(), 3);

    dv.Print(ss, "");
    EXPECT_EQ(ss.str(), "(M, Dan4ick)(M, Misha)(F, Masha)");
}
