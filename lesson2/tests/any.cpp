#include <gtest/gtest.h>

#include <any>
#include <fmt/core.h>

namespace detail {
  // Here is a possible implementation of std::any
  class my_any {
  public:
    template<typename T>
    explicit my_any(T value) : obj_{std::make_unique<handler<T>>(std::move(value))} {}

    decltype(auto) get() { return obj_->get(); }

    template<typename T>
    auto& cast_to(){
//      auto &type_obj = dynamic_cast<handler<T>&>(*obj_);
//      return *static_cast<T*>(type_obj.get());

      if (typeid(T) == obj_->type())
        return *static_cast<T*>(get());

      throw std::bad_any_cast{};
    }

    // HW: Add move/copy-assignment operators
    // HW: Add copy/move-ctor

  private:
    struct base {
      virtual void *get() = 0;

      virtual const std::type_info& type() const = 0;

      virtual ~base() = default;
    };

    template<typename T>
    struct handler : base {
      explicit handler(T value) : var{std::move(value)} {}

      const std::type_info& type() const override { return typeid(T); }

      void *get() override { return &var; }

      T var;
    };

    std::unique_ptr<base> obj_;
  };

  template<typename T>
  T &any_cast(my_any &v) {
    // HW: Implement cast function to get the value of type T from my_any
  }

  // HW: Write tests for my_any class

}// namespace detail

TEST(Any, ConstructionAndAccess) {
  std::any a = 10;

  ASSERT_NO_THROW(std::any_cast<int>(a));
  ASSERT_THROW(std::any_cast<std::string>(a), std::bad_any_cast);

  ASSERT_EQ(std::any_cast<int>(a), 10);

  auto golden = std::string{"Here is a string in std::any"};
  a = golden;

  ASSERT_THROW(std::any_cast<int>(a), std::bad_any_cast);
  ASSERT_NO_THROW(std::any_cast<std::string>(a));

  ASSERT_EQ(std::any_cast<std::string>(a), golden);
}

namespace {
  auto process_any(const std::any &v) {
    auto dump_any_content = [](const auto &v) {
      fmt::print("std::any handle type {} with value {}\n",
                 typeid(decltype(v)).name(), v);
    };
    if (std::any_cast<int>(&v) != nullptr) {
      dump_any_content(std::any_cast<int>(v));
    } else if (std::any_cast<double>(&v) != nullptr) {
      dump_any_content(std::any_cast<double>(v));
    } else if (std::any_cast<std::string>(&v) != nullptr) {
      dump_any_content(std::any_cast<std::string>(v));
    } else {
      fmt::print("Unknown any.\n");
    }
  }
}// namespace

// HW: Suggest the output from this test.
TEST(Any, InvokeFunction) {
  std::any v = 10;
  ::process_any(v);

  v = 3.14;
  ::process_any(v);

  v = "Some string value (or not)";
  ::process_any(v);

  v = std::string{"Some string value (or not)"};
  ::process_any(v);
}