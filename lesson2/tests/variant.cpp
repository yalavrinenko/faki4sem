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

class Visitor : public ::testing::Test {
protected:
  std::variant<int, double, std::string> var1 = 10;
  std::variant<int, double, std::string> var2 = 10.89;
  std::variant<int, double, std::string> var3 = "Awesome str";
};

TEST_F(Visitor, FunctionalObject) {
  struct {
    std::string operator()(int value) const {
      fmt::print("We have int variant ({})! {}\n", __PRETTY_FUNCTION__, value);
      return "int";
    }

    std::string operator()(double value) const {
      fmt::print("We have double variant! ({}) {}\n", __PRETTY_FUNCTION__,
                 value);
      return "double";
    }

    std::string operator()(const std::string &str) const {
      fmt::print("We have std::string in variant! ({}) {}\n",
                 __PRETTY_FUNCTION__, str);
      return "std::string";
    }
  } variant_visitor;

  fmt::print("Visit to variant. Result = {}\n",
             std::visit(variant_visitor, var1));
  fmt::print("Visit to variant. Result = {}\n",
             std::visit(variant_visitor, var2));
  fmt::print("Visit to variant. Result = {}\n",
             std::visit(variant_visitor, var3));
}

template<typename... T>
struct Overloads : T... {
  using T::operator()...;
};
template<typename... T>
Overloads(T...) -> Overloads<T...>;

TEST_F(Visitor, LambdaObject) {
  auto visitor =
      Overloads{[](int value) {
                  fmt::print("We have int variant ({})! {}\n",
                             __PRETTY_FUNCTION__, value);
                  return "int";
                },
                [](double value) {
                  fmt::print("We have double variant! ({}) {}\n",
                             __PRETTY_FUNCTION__, value);
                  return "double";
                },
                [](const std::string &str) {
                  fmt::print("We have std::string in variant! ({}) {}\n",
                             __PRETTY_FUNCTION__, str);
                  return "std::string";
                }};

  fmt::print("Visit to variant. Result = {}\n", std::visit(visitor, var1));
  fmt::print("Visit to variant. Result = {}\n", std::visit(visitor, var2));
  fmt::print("Visit to variant. Result = {}\n", std::visit(visitor, var3));
}

TEST_F(Visitor, LambdaAggregation) {
  auto visitor = [](auto &&value) {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, int>) {
      fmt::print("We have int variant ({})! {}\n", __PRETTY_FUNCTION__, value);
      return "int";
    }
    if constexpr (std::is_same_v<T, double>) {
      fmt::print("We have double variant! ({}) {}\n", __PRETTY_FUNCTION__,
                 value);
      return "double";
    }
    if constexpr (std::is_same_v<T, std::string>) {
      fmt::print("We have std::string in variant! ({}) {}\n",
                 __PRETTY_FUNCTION__, value);
      return "std::string";
    }
  };

  fmt::print("Visit to variant. Result = {}\n", std::visit(visitor, var1));
  fmt::print("Visit to variant. Result = {}\n", std::visit(visitor, var2));
  fmt::print("Visit to variant. Result = {}\n", std::visit(visitor, var3));
}