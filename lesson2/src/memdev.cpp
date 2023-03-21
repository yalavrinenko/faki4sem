#include "memdev.hpp"
#include "ranked_device.hpp"
#include "sequential_device.hpp"

namespace cooldev::memory {
  device make_device(device_type type, uint64_t size) {
    if (type == device_type::sequential)
      return device(std::make_unique<device_seq_impl>(size));
    if (type == device_type::ndimensional)
      return device(std::make_unique<device_ranked_impl>(size));

    throw std::logic_error{"Wrong device type"};
  }
}// namespace cooldev::memory