#include <gtest/gtest.h>

// Um teste inicial para garantir que o GTest e a integracao com o CMake funcionam perfeitamente.
TEST(DummyTest, GTestEstaConfigurado) {
    EXPECT_TRUE(true);
    EXPECT_EQ(1 + 1, 2);
}
