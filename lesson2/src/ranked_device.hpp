#ifndef CMAKE_DEMO_RANKED_DEVICE_HPP
#define CMAKE_DEMO_RANKED_DEVICE_HPP

#include "memdev.hpp"

namespace cooldev::memory {
  class device_ranked_impl : public implementation_base {
  public:
    explicit device_ranked_impl(uint64_t size);

    ~device_ranked_impl() override;

  private:
    [[nodiscard]] address space_() const override { return addr_; }

    nd_address addr_{};
  };
}// namespace cooldev::memory

#endif//CMAKE_DEMO_RANKED_DEVICE_HPP
