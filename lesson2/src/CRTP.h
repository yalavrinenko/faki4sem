//
// Created by yalavrinenko on 04.04.23.
//

#ifndef CMAKE_DEMO_CRTP_H
#define CMAKE_DEMO_CRTP_H

#include <fmt/core.h>

template<typename Derived>
class Parser {
public:
  void common() const {
    fmt::print("Call common function from {}\n", __PRETTY_FUNCTION__);
  }

  void parse() const { static_cast<const Derived *>(this)->parse_(); }

  void get() const { static_cast<const Derived *>(this)->get_(); }

  decltype(auto) compile() const {
    return static_cast<const Derived *>(this)->compile_();
  }
};

#endif//CMAKE_DEMO_CRTP_H
