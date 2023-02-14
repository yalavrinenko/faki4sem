#include <type_traits>

#include <fmt/core.h>

#include "A/libA.hpp"
#include "B/libB.hpp"

#include "CopyElisionDemo/demo_copy_elision.hpp"

int main() {
  fmt::print("Message from lib A: {}\n", A::get_message_from_lib_A());
  fmt::print("Message from lib B: {}\n", B::get_message_from_lib_B(42));

  lesson1::demo::show_copy_elision();
  lesson1::demo::show_copy_elision_fail();
  return 0;
}
