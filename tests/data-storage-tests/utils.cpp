#include "pch.h"
#include "utils.h"

void CHECK_EQ(std::vector<DataRow>& expected, const DataContainerPtr container) {
    EXPECT_EQ(expected.size(), container->RowsCount());

    std::stringstream ss_expected;
    std::stringstream ss_actual;
    for (size_t row_num = 0; row_num < container->RowsCount(); ++row_num) {
        ss_expected << expected[row_num].ToString() << std::endl;
        ss_actual << container->GetRow(row_num)->GetRow().ToString() << std::endl;
    }

    EXPECT_EQ(ss_expected.str(), ss_actual.str());
}