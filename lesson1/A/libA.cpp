
#include "libA.hpp"

const char *A::get_message_from_lib_A() {
  // string literal has static storage duration. pointer can be returned safely
  const char *msg = "Hello from lib A";
  return msg;
}