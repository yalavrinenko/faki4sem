#ifndef CMAKE_DEMO_MEMDEV_HPP
#define CMAKE_DEMO_MEMDEV_HPP

#include <array>
#include <cstdint>
#include <memory>
#include <variant>
#include <unordered_map>

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

  class device;
  device make_device(device_type type, uint64_t size);

  class device {
  public:

    static device& create(device_type type, uint64_t size){
      static std::unordered_map<device_type, device> device_pool;
      if (!device_pool.contains(type))
        device_pool.emplace(type, make_device(type, size));
      return device_pool.at(type);
    }

    [[nodiscard]] decltype(auto) space() const {
      return impl_->space();
    }

  private:
    friend device make_device(device_type type, uint64_t size);

    template<typename Implementation>
    explicit device(std::unique_ptr<Implementation> impl)
        : impl_{std::move(impl)} {}

  private:
    std::unique_ptr<implementation_base> impl_;
  };
}// namespace cooldev::memory

#endif//CMAKE_DEMO_MEMDEV_HPP
