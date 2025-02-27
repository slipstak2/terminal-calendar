#include "pch.h"
#include "utils.h"

void CHECK_EQ(const std::vector<DataRow>& expected, const DataContainerPtr container) {
    EXPECT_EQ(expected.size(), container->RowsCount());

    std::stringstream ss_expected;
    for (size_t row_num = 0; row_num < expected.size(); ++row_num) {
        ss_expected << expected[row_num].ToString() << std::endl;
    }

    EXPECT_EQ(ss_expected.str(), container->ToString());
}


void CHECK_EQ(const DataContainerPtr lhs, const DataContainerPtr rhs) {
    EXPECT_EQ(lhs->RowsCount(), rhs->RowsCount());
    EXPECT_EQ(lhs->FieldsCount(), rhs->FieldsCount());

    EXPECT_EQ(lhs->ToString(), rhs->ToString());
}