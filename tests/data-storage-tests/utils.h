#pragma once

#include "data-container.h"
#include "data-row.h"
#include <vector>

void CHECK_EQ(const std::vector<DataRow>& expected, const DataContainerPtr container);
void CHECK_EQ(const DataContainerPtr lhs, const DataContainerPtr rhs);

