#ifndef CMAKE_DEMO_MEMDEV_HPP
#define CMAKE_DEMO_MEMDEV_HPP

#include <array>
#include <cstdint>
#include <memory>
#include <variant>

namespace cooldev::memory {
  enum class device_type { sequential, ndimensional };

  using sq_address = uint64_t *;
  inline constexpr auto dim_number = 4;
  using nd_address = std::array<sq_address, dim_number>;

  using address = std::variant<sq_address, nd_address>;

  class implementation_base {
  public:
    [[nodiscard]] address space() const { return space_(); }

    virtual ~implementation_base() = default;

  private:
    [[nodiscard]] virtual address space_() const = 0;
  };

  class device {
  public:
    template<typename Implementation>
    explicit device(std::unique_ptr<Implementation> impl)
        : impl_{std::move(impl)} {}

    [[nodiscard]] decltype(auto) space() const { return impl_->space(); }

  private:
    std::unique_ptr<implementation_base> impl_;
  };

  device make_device(device_type type, uint64_t size);
}// namespace cooldev::memory

#endif//CMAKE_DEMO_MEMDEV_HPP
