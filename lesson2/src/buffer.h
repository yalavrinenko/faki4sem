#ifndef CMAKE_DEMO_BUFFER_H
#define CMAKE_DEMO_BUFFER_H
#include "memdev.hpp"

template<typename T>
struct ProxyObject;

class buffer_implementation {
public:
  template<typename T>
  void read(size_t index, T &value){
    read_(index * sizeof(T), &value, sizeof(T));
  }

  template<typename T>
  void write(size_t index, const T& value){
    write_(index * sizeof(T), &value, sizeof(T));
  }

private:
  virtual void read_(size_t byte, void *value, size_t size) = 0;
  virtual void write_(size_t byte, const void *value, size_t size) = 0;
};

class seq_buffer_impl : public buffer_implementation{

};

class nd_buffer_impl : public buffer_implementation{

};

template<typename T>
class buffer {
public:
  explicit buffer(cooldev::memory::address addr) {
//    auto creator = [](auto &&addr){
//      using T = std::decay_t(decltype(addr));
//      if constexpr (std::is_same_v<T, cooldev::memory::sq_address){
//        // create seq impl
//      } else
//        // create nd- impl
//    };
//
//    std::visit(creator, addr);
  }

  T operator[](size_t i) const {
    T value;
    impl_->read(i, value);
    return value;
  }

  ProxyObject<T> operator[](size_t i) { return ProxyObject<T>(*this, i); }

  void store(size_t i, const T &value) {
    impl_->write(i, value);
  }

private:
  std::unique_ptr<buffer_implementation> impl_;
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

#endif//CMAKE_DEMO_BUFFER_H
