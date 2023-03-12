#include <gtest/gtest.h>

#include <fmt/core.h>
#include <variant>

TEST(Variant, ConstructionAndAccess) {
  std::variant<int, std::string> a = 10;

  ASSERT_NO_THROW(std::get<int>(a));
  ASSERT_THROW(std::get<std::string>(a), std::bad_variant_access);

  ASSERT_EQ(std::get<int>(a), 10);

  auto golden = std::string{"Here is a string in std::any"};
  a = golden;

  ASSERT_THROW(std::get<int>(a), std::bad_variant_access);
  ASSERT_NO_THROW(std::get<std::string>(a));

  ASSERT_EQ(std::get<std::string>(a), golden);
}

namespace {
  auto
  process_variant_trivial(const std::variant<int, double, std::string> &v) {
    auto dump_any_content = [](const auto &v) {
      fmt::print("std::any handle type {} with value {}\n",
                 typeid(decltype(v)).name(), v);
    };
    if (std::get_if<int>(&v) != nullptr) {
      dump_any_content(std::get<int>(v));
    } else if (std::get_if<double>(&v) != nullptr) {
      dump_any_content(std::get<double>(v));
    } else if (std::get_if<std::string>(&v) != nullptr) {
      dump_any_content(std::get<std::string>(v));
    }
  }
}// namespace

// HW: Suggest the output from this test.
TEST(Variant, InvokeFunction) {
  std::variant<int, double, std::string> v = 10;
  ::process_variant_trivial(v);

  v = 3.14;
  ::process_variant_trivial(v);

  // HW: Why the next line is correct?
  v = "Some string value (or not)";
  ::process_variant_trivial(v);

  v = std::string{"Some string value (or not)"};
  ::process_variant_trivial(v);
}