
#include "gtest/gtest.h"

static int test_math(int a, int b)
{
        return a * b;
}

namespace {
        TEST(test_math, HandlesPositiveInput)
        {
                EXPECT_EQ(test_math(4, 4), 16);	
        }

        TEST(test_math, HandleNegativeInput)
        {
                EXPECT_EQ(test_math(9, -2), -18);	
        }
}

int main()
{
        testing::InitGoogleTest();
        return RUN_ALL_TESTS();
}