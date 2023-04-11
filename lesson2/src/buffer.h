#ifndef CMAKE_DEMO_BUFFER_H
#define CMAKE_DEMO_BUFFER_H
#include "memdev.hpp"
#include <algorithm>
#include <iterator>

template<typename T>
struct ProxyObject;

class buffer_implementation {
public:
  template<typename T>
  void read(size_t index, T &value) {
    read_(index * sizeof(T), &value, sizeof(T));
  }

  template<typename T>
  void write(size_t index, const T &value) {
    write_(index * sizeof(T), &value, sizeof(T));
  }

  virtual ~buffer_implementation() = default;

private:
  virtual void read_(size_t byte, void *value, size_t size) = 0;
  virtual void write_(size_t byte, const void *value, size_t size) = 0;
};

class seq_buffer_impl : public buffer_implementation {
public:
  seq_buffer_impl(cooldev::memory::sq_address addr, uint64_t size)
      : base_{addr}, size_{size} {}

private:
  void read_(size_t byte, void *value, size_t size) override {
    auto *dst = static_cast<uint8_t *>(value);
    const auto *read_base = reinterpret_cast<const uint8_t *>(base_) + byte;

    std::copy_n(read_base, size, dst);
  }

  void write_(size_t byte, const void *value, size_t size) override {
    const auto *src = static_cast<const uint8_t *>(value);

    auto *dst = reinterpret_cast<uint8_t *>(base_) + byte;
    std::copy(src, src + size, dst);
  }

  cooldev::memory::sq_address base_;
  [[maybe_unused]] uint64_t size_;
};

class nd_buffer_impl : public buffer_implementation {
public:
  nd_buffer_impl(cooldev::memory::nd_address addr, uint64_t size)
      : base_{addr}, size_{size} {}

private:
  void read_(size_t byte, void *value, size_t size) override {
    static constexpr auto rank_for_read = 0;
    auto *dst = static_cast<uint8_t *>(value);
    const auto *read_base =
        reinterpret_cast<const uint8_t *>(base_[rank_for_read]) + byte;

    std::copy_n(read_base, size, dst);
  }

  void write_(size_t byte, const void *value, size_t size) override {
    const auto *src = static_cast<const uint8_t *>(value);
    for (auto &rank_start: base_) {
      auto *dst = reinterpret_cast<uint8_t *>(rank_start) + byte;
      std::copy(src, src + size, dst);
    }
  }

  cooldev::memory::nd_address base_;
  [[maybe_unused]] uint64_t size_;
};

template<typename T, typename Reference>
class Iterator;

template<typename T>
class buffer {
public:
  explicit buffer(cooldev::memory::address addr, uint64_t size) : size_{size} {
    auto creator =
        [size](auto &&addr) -> std::unique_ptr<buffer_implementation> {
      using U = std::decay_t<decltype(addr)>;
      if constexpr (std::is_same_v<U, cooldev::memory::sq_address>) {
        return std::make_unique<seq_buffer_impl>(addr, size);
      } else
        return std::make_unique<nd_buffer_impl>(addr, size);
    };

    impl_ = std::visit(creator, addr);
  }

  T operator[](size_t i) const {
    T value;
    impl_->read(i, value);
    return value;
  }

  ProxyObject<T> operator[](size_t i) { return ProxyObject<T>(*this, i); }

  void store(size_t i, const T &value) { impl_->write(i, value); }

  auto begin() { return Iterator<T, ProxyObject<T>>(this, 0); }

  auto end() { return Iterator<T, ProxyObject<T>>(this, size_); }

  auto begin() const { return Iterator<T, T>(this, 0); }

  auto end() const { return Iterator<T, T>(this, size_); }

private:
  std::unique_ptr<buffer_implementation> impl_;
  uint64_t size_;
};

template<typename T>
struct ProxyObject {
  ProxyObject(buffer<T> &parent, size_t index)
      : index_(index), parent_(parent) {}

  ProxyObject &operator=(const T &value) {
    parent_.store(index_, value);
    return *this;
  }

  ProxyObject &operator=(const ProxyObject<T> &other) {
    parent_.store(index_, T(other));
    return *this;
  }

  operator T() const { return std::cref(parent_).get()[index_]; }

private:
  size_t index_;
  buffer<T> &parent_;
};

template<typename T, typename Reference>
class Iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using reference = Reference;
  using difference_type = size_t;

public:
  bool operator!=(const Iterator<T, Reference> &rhs) const {
    return (base_ == rhs.base_) && (index_ != rhs.index_);
  }

  reference operator*() { return base_->operator[](index_); }

  reference operator*() const { return base_->operator[](index_); }

  reference operator->() { return base_->operator[](index_); }

  reference operator->() const { return base_->operator[](index_); }

  auto &operator++() {
    ++index_;
    return *this;
  }

  auto &operator+=(difference_type n) {
    index_ += n;
    return *this;
  }

protected:
  Iterator(buffer<T> *base, uint64_t index) : base_{base}, index_{index} {}

private:
  friend class buffer<T>;

  buffer<T> *base_;
  uint64_t index_ = 0;
};

#endif//CMAKE_DEMO_BUFFER_H
