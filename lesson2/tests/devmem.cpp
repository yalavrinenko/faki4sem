#include "src/memdev.hpp"
#include "src/buffer.h"
#include <fmt/core.h>

#include <gtest/gtest.h>

TEST(CommonDevice, Sequential) {
  auto device = cooldev::memory::make_device(
      cooldev::memory::device_type::sequential, 1024);

  auto addr = device.space();

  ASSERT_NO_THROW(std::get<cooldev::memory::sq_address>(addr));

  ASSERT_THROW(std::get<cooldev::memory::nd_address>(addr),
               std::bad_variant_access);
}

TEST(CommonDevice, Ranked) {
  auto device = cooldev::memory::make_device(
      cooldev::memory::device_type::ndimensional, 1024);

  auto addr = device.space();

  ASSERT_NO_THROW(std::get<cooldev::memory::nd_address>(addr));

  ASSERT_THROW(std::get<cooldev::memory::sq_address>(addr),
               std::bad_variant_access);
}

//We want to have next code
TEST(CommontDevice, Access) {
  auto device = cooldev::memory::make_device(
      cooldev::memory::device_type::ndimensional, 1024);


  buffer<int> buf(std::get<cooldev::memory::nd_address>(device.space()));

  buf[10] = 0;
  ASSERT_EQ(buf[10], 0);

  buf[10] = 5;

  if (buf[10] == 5){
    fmt::print("Equal");
  }

  ASSERT_EQ(buf[10], 5);

  buf[10] = buf[10] * 2;
  ASSERT_EQ(buf[10], 10);


  buf[5] = buf[10];
  ASSERT_EQ(buf[5], buf[10]);
}