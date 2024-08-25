#include <gtest/gtest.h>

TEST(NullTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(4*6, 24);
}