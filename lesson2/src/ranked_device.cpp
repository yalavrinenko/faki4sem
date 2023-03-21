#include "ranked_device.hpp"

cooldev::memory::device_ranked_impl::device_ranked_impl(uint64_t size) {
  for (auto &saddr: addr_) { saddr = new uint64_t[size]; }
}

cooldev::memory::device_ranked_impl::~device_ranked_impl() {
  for (auto &saddr: addr_) { delete[] saddr; }
}
