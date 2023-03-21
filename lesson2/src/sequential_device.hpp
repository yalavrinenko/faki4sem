#ifndef CMAKE_DEMO_SEQUENTIAL_DEVICE_HPP
#define CMAKE_DEMO_SEQUENTIAL_DEVICE_HPP

#include "memdev.hpp"

namespace cooldev::memory {
  class device_seq_impl : public implementation_base {
  public:
    explicit device_seq_impl(uint64_t size);

    ~device_seq_impl() override;

  private:
    [[nodiscard]] address space_() const override { return addr_; }

    sq_address addr_;
  };
}// namespace cooldev::memory

#endif//CMAKE_DEMO_SEQUENTIAL_DEVICE_HPP
