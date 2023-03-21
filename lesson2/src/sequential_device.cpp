#include "sequential_device.hpp"

#include <algorithm>

cooldev::memory::device_seq_impl::device_seq_impl(uint64_t size)
    : addr_{new uint64_t[size]} {
  std::fill(addr_, addr_ + size, 0);
}

cooldev::memory::device_seq_impl::~device_seq_impl() { delete[] addr_; }
