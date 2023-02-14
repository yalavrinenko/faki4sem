#include "demo_copy_elision.hpp"

#include <fmt/core.h>

lesson1::demo::ProbeClass::ProbeClass() {
  fmt::print("{}\n", __PRETTY_FUNCTION__);
}

lesson1::demo::ProbeClass::ProbeClass(const lesson1::demo::ProbeClass &) {
  fmt::print("WARNING!!! {}\n", __PRETTY_FUNCTION__);
}

lesson1::demo::ProbeClass::~ProbeClass() {
  fmt::print("{}\n", __PRETTY_FUNCTION__);
}

lesson1::demo::ProbeClass lesson1::demo::CopyElision::operator()() const {
  ProbeClass obj;
  obj.update_state(100);

  fmt::print("Address of object in {} is {}\n", __PRETTY_FUNCTION__,
             static_cast<const void *>(&obj));

  return obj;
}

const lesson1::demo::ProbeClass &
lesson1::demo::CopyElisionFailed::operator()() const {
  static ProbeClass obj;
  obj.update_state(100);

  fmt::print("Address of object in {} is {}\n", __PRETTY_FUNCTION__,
             static_cast<const void *>(&obj));

  return obj;
}

void lesson1::demo::show_copy_elision() {
  fmt::print("\n\n=====Here we have copy elision=====\n");

  CopyElision creator;
  auto value = creator();

  fmt::print("Address of object in {} is {}\n", __PRETTY_FUNCTION__,
             static_cast<const void *>(&value));
}

void lesson1::demo::show_copy_elision_fail() {
  fmt::print("\n\n=====Here the copy elision fails=====\n");

  CopyElisionFailed creator;
  auto value = creator();

  fmt::print("Address of object in {} is {}\n", __PRETTY_FUNCTION__,
             static_cast<const void *>(&value));
}

//HW: try to remove code duplication for function show_copy_elision_fail() and show_copy_elision()