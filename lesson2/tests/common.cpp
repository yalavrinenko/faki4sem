#include "src/common.hpp"

#include <gtest/gtest.h>

TEST(CommonDevice, Sequential) {
  auto device = cooldev::memory::v2::make_device(
      cooldev::memory::device_type::sequential, 1024);

  auto addr = device.space();

  ASSERT_NO_THROW(std::get<cooldev::memory::sq_address>(addr));

  ASSERT_THROW(std::get<cooldev::memory::nd_address>(addr),
               std::bad_variant_access);
}

TEST(CommonDevice, Ranked) {
  auto device = cooldev::memory::v2::make_device(
      cooldev::memory::device_type::ndimensional, 1024);

  auto addr = device.space();

  ASSERT_NO_THROW(std::get<cooldev::memory::nd_address>(addr));

  ASSERT_THROW(std::get<cooldev::memory::sq_address>(addr),
               std::bad_variant_access);
}

TEST(CommontDevice, Access) {
  auto device = cooldev::memory::v2::make_device(
      cooldev::memory::device_type::ndimensional, 1024);

  auto acc = device.get_direct_access<int>();

  acc[10] = 45;
  acc[11] = acc[10] + acc[78] * 5;
  for (auto v: acc) {}

  std::copy(acc.begin(), acc.end(),
            std::ostream_iterator<int>(std::cout, "\n"));
}