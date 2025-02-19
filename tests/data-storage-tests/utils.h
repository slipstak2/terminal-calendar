#pragma once

#include "data-container.h"
#include "data-row.h"
#include <vector>

void CHECK_EQ(std::vector<DataRow>& expected, const DataContainerPtr container);
