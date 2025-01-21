#include "pch.h"

#include "data-storage.h"
#include <tuple>


TEST(TestDataSet, Empty) {
    using namespace exp1;

    DataStorage<std::string, int, char> storage;
    DataView<decltype(storage), 2, 0> dv(storage);
    EXPECT_EQ(dv.Size(), 0);
}

TEST(TestDataSet, Simple) {
    using namespace exp1;

    DataStorage<std::string, int> storage;
    storage.Add(std::make_tuple<std::string, int>("Dan4ick", 28));
    storage.Add(std::make_tuple<std::string, int>("Misha", 25));
    storage.Add(std::make_tuple<std::string, int>("Igor", 38));

    EXPECT_EQ(storage.Size(), 3);
    DataView<decltype(storage), 1, 0, 1> dv(storage);
    DataSet<decltype(dv), DataStorage<int>> dataSet(dv);

    auto Dan4ick = dataSet.Get(0);
    std::stringstream ss;
    
    print_tuple_pretty(Dan4ick, ss);
    EXPECT_EQ(ss.str(), "(28, Dan4ick, 28)");
}
