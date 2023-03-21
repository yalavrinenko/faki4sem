#ifndef CMAKE_DEMO_COMMON_HPP
#define CMAKE_DEMO_COMMON_HPP

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

  namespace v1 {
    class device {
    public:
      [[nodiscard]] address space() const { return space_(); }

      virtual ~device() = default;

    private:
      [[nodiscard]] virtual address space_() const = 0;
    };

  }// namespace v1

  namespace v2 {
    class implementation_base {
    public:
      [[nodiscard]] address space() const { return space_(); }

      virtual ~implementation_base() = default;

    private:
      [[nodiscard]] virtual address space_() const = 0;
    };

    class device_seq_impl : public implementation_base {
    public:
      explicit device_seq_impl(uint64_t size) : addr_{new uint64_t[size]} {}

      ~device_seq_impl() override { delete[] addr_; }

    private:
      [[nodiscard]] address space_() const override { return addr_; }
      sq_address addr_;
    };

    class device_ranked_impl : public implementation_base {
    public:
      explicit device_ranked_impl(uint64_t size) {
        for (auto &saddr : addr_){
          saddr = new uint64_t[size];
        }
      }

      ~device_ranked_impl() override {
        for (auto &saddr : addr_){
          delete[] saddr;
        }
      }

    private:
      [[nodiscard]] address space_() const override { return addr_; }
      nd_address addr_{};
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

    auto make_device(device_type type, uint64_t size){
      if (type == device_type::sequential)
        return device(std::make_unique<device_seq_impl>(size));
      if (type == device_type::ndimensional)
        return device(std::make_unique<device_ranked_impl>(size));

      throw std::logic_error{"Wrong device type"};
    }
  }// namespace v2


  /*
   * HW: Оцените преимущества и недостатки классов device в
   * пространствах имён v1 и v2. Приведите примеры использования классов
   * v1::device и v2::device.
   * */

}// namespace cooldev::memory

#endif//CMAKE_DEMO_COMMON_HPP
