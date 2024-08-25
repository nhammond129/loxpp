#include <gtest/gtest.h>

TEST(example, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(4*6, 24);
}