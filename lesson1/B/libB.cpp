#include "libB.hpp"

std::string B::get_message_from_lib_B(unsigned long value) {
  std::string msg = "Hello " + std::to_string(value) + " times from lib B";

  // HW: Try to change the internals of the function and recompile only libB
  return msg;
}