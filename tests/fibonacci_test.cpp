/**
 * @file fibonacci_test.cpp
 * @brief Tests for the Fibonacci function
 */

#include <gtest/gtest.h>
#include "fibonacci.hpp"

using namespace mycpplibrary;

TEST(FibonacciTest, ZeroInput) {
    EXPECT_EQ(fibonacci(0), 0);
}

TEST(FibonacciTest, OneInput) {
    EXPECT_EQ(fibonacci(1), 1);
}

TEST(FibonacciTest, SmallInputs) {
    EXPECT_EQ(fibonacci(2), 1);
    EXPECT_EQ(fibonacci(3), 2);
    EXPECT_EQ(fibonacci(4), 3);
    EXPECT_EQ(fibonacci(5), 5);
    EXPECT_EQ(fibonacci(6), 8);
    EXPECT_EQ(fibonacci(7), 13);
}

TEST(FibonacciTest, LargerInput) {
    EXPECT_EQ(fibonacci(10), 55);
    EXPECT_EQ(fibonacci(15), 610);
    EXPECT_EQ(fibonacci(20), 6765);
}

TEST(FibonacciTest, NegativeInput) {
    // Negative inputs should return 0 according to our implementation
    EXPECT_EQ(fibonacci(-1), 0);
    EXPECT_EQ(fibonacci(-5), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
