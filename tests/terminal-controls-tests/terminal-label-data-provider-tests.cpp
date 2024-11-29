#include "pch.h"
#include <Windows.h>

#include "TerminalLabelDataProvider.h"
#include "DataProviders/ListDataProvider.h"

class LabelDataProviderTests : public testing::Test
{
protected:
    LabelDataProviderTests() {
        SetConsoleOutputCP(CP_UTF8);
    }
    ~LabelDataProviderTests() override { }
};

TEST_F(LabelDataProviderTests, Empty) {

    auto labelDataProvider = TerminalLabelDataProvider::Create(nullptr, TerminalCoord());
    EXPECT_EQ(labelDataProvider->GetKind(), TerminalControl::Kind::LABEL_DATA_PROVIDER);
    EXPECT_TRUE(labelDataProvider->IsKindOf(TerminalControl::Kind::LABEL_BASE));
}

TEST_F(LabelDataProviderTests, Elements3) {
    std::vector<Utf8String> data{ "Первый", "-Второй-", "Third" };

    auto dataProvider = ListDataProvider::Create(data);
    auto labelDataProvider = TerminalLabelDataProvider::Create(dataProvider, TerminalCoord());
    EXPECT_EQ(data[1].size(), labelDataProvider->Length()); // 8

    EXPECT_EQ(data[0], labelDataProvider->Get());
    EXPECT_FALSE(dataProvider->HasPrev());
    EXPECT_TRUE(dataProvider->HasNext());

    EXPECT_TRUE(dataProvider->Next());

    EXPECT_EQ(data[1], labelDataProvider->Get());
    EXPECT_TRUE(dataProvider->HasPrev());
    EXPECT_TRUE(dataProvider->HasNext());

    EXPECT_TRUE(dataProvider->Next());

    EXPECT_EQ(data[2], labelDataProvider->Get());
    EXPECT_TRUE(dataProvider->HasPrev());
    EXPECT_FALSE(dataProvider->HasNext());

    EXPECT_FALSE(dataProvider->Next());
    EXPECT_TRUE(dataProvider->Prev());

    EXPECT_EQ(data[1], labelDataProvider->Get());
    EXPECT_TRUE(dataProvider->Prev());
    EXPECT_EQ(data[0], labelDataProvider->Get());
    EXPECT_FALSE(dataProvider->Prev());
}