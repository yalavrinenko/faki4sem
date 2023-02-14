#include <gtest/gtest.h>

#include "A/libA.hpp"
#include "B/libB.hpp"

TEST(MessageCheck, LibA) {
  const std::string golden = "Hello from lib A";
  ASSERT_EQ(A::get_message_from_lib_A(), golden);
}

TEST(MessageCheck, LibB) {
  const std::string golden = "Hello 42 times from lib B";
  ASSERT_EQ(B::get_message_from_lib_B(42), golden);
}