#include <gtest/gtest.h>

#include <cstdint>
#include <fmt/core.h>
#include <fmt/format.h>

template<typename T, uint64_t Align, uint64_t Size>
constexpr auto storage_size() {
  auto estimated_size = sizeof(T) + Align;
  return (estimated_size + Size - 1) & (~(Size - 1));
}

template<uint64_t Align, uint64_t Size, typename T>
class AlignedStorage {
public:
  template<typename... Args>
  constexpr explicit AlignedStorage(Args &&...args) {
    uint64_t space = sizeof(storage_);
    base_ = storage_;
    std::align(Align, sizeof(T), base_, space);

    new (base_) T(std::forward<Args>(args)...);
  }

  constexpr T &operator*() { return *static_cast<T *>(base_); }
  constexpr const T &operator*() const {
    return *static_cast<const T *>(base_);
  }

  constexpr ~AlignedStorage() { static_cast<T *>(base_)->~T(); }

private:
  uint8_t storage_[storage_size<T, Align, Size>()] = {};
  void *base_ = nullptr;
};

// For each test's entry check the next values of Align and Size
// Align - 1, 2, 4, 8, 16, 32
// Size - 1, 2, 8, 32, 64, 128

template<uint64_t Align, uint64_t Size, typename T, typename... Args>
constexpr auto make_storage(Args &&...args) {// HW: Read about constexpr
  return AlignedStorage<Align, Size, T>{std::forward<Args>(args)...};
}

TEST(AlignedStorageTest, Easy) {
  static constexpr auto Align = 8UL;
  static constexpr auto Size = 16UL;

  auto value = make_storage<Align, Size, uint32_t>(42);
  const char dummy[] = {'S', 't', 'a', 'c', ' ', 'i', 's', ' ', 'c',
                        'o', 'r', 'r', 'u', 'p', 't', 'e', 'd'};

  //Check the object alignment
  ASSERT_EQ(reinterpret_cast<uintptr_t>(&(*value)) % Align, 0);
  //Check the object size
  std::vector<uint8_t> mem_dump(Size, 0);
  std::copy_n(reinterpret_cast<const uint8_t *>(&(*value)), Size,
              mem_dump.begin());
  for (auto i = sizeof(*value); i < mem_dump.size(); ++i)
    ASSERT_EQ(std::find(dummy, dummy + sizeof(dummy), mem_dump[i]),
              dummy + sizeof(dummy));

  // HW: Think how we can simplify check above.
  // HW: Make a suggestion how we can check that copy operation is correct.

  fmt::print("Memory dump: {:X}\n", fmt::join(mem_dump, ""));

  fmt::print("Storage address: {}\nObject address: {}\nStorage size: {}\n",
             static_cast<void *>(&value), static_cast<void *>(&(*value)),
             sizeof(value));

  ASSERT_EQ(*value, 42);
  *value = 10;
  ASSERT_EQ(*value, 10);

  // HW: Make similar checks for next data types: char, uint64_t, std::string, std::vector
}

TEST(AlignedStorageTest, Medium) {
  static constexpr auto Align = 64UL;
  static constexpr auto Size = 16UL;

  struct ProbeObject {
    ProbeObject(uint32_t v1, uint64_t v2, uint8_t v3)
        : value_1{v1}, value_2{v2}, value_3{v3} {}
    uint32_t value_1;
    uint64_t value_2;
    uint8_t value_3;
  };
  // HW: sizeof(ProbeObject) is ???
  // HW: Why sizeof(ProbeObject) is not equal to sizeof(uint8_t) + sizeof(uint64_t) + sizeof(uint32_t);

  auto value = make_storage<Align, Size, ProbeObject>(43, 14, 28);
  const char dummy[] = {'S', 't', 'a', 'c', ' ', 'i', 's', ' ', 'c',
                        'o', 'r', 'r', 'u', 'p', 't', 'e', 'd'};

  //Check the object alignment
  ASSERT_EQ(reinterpret_cast<uintptr_t>(&(*value)) % Align, 0);
  //Check the object size
  std::vector<uint8_t> mem_dump(Size, 0);
  std::copy_n(reinterpret_cast<const uint8_t *>(&(*value)), Size,
              mem_dump.begin());
  for (auto i = sizeof(*value); i < mem_dump.size(); ++i)
    ASSERT_EQ(std::find(dummy, dummy + sizeof(dummy), mem_dump[i]),
              dummy + sizeof(dummy));

  // HW: Think how we can simplify check above.
  // HW: Make a suggestion how we can check that copy operation is correct.

  fmt::print("Memory dump: {:X}\n", fmt::join(mem_dump, ""));

  fmt::print("Storage address: {}\nObject address: {}\nStorage size: {}\n",
             static_cast<void *>(&value), static_cast<void *>(&(*value)),
             sizeof(value));

  auto &obj = *value;

  ASSERT_EQ(obj.value_1, 43);
  ASSERT_EQ(obj.value_2, 14);
  ASSERT_EQ(obj.value_3, 28);
}

class ProbeObject {
public:
  explicit ProbeObject(std::vector<double> v) : v_{std::move(v)} {
    ++ctor_calls_;
  }

  ProbeObject(const ProbeObject &other) : v_{other.v_} { ++copy_calls_; }
  ProbeObject &operator=(const ProbeObject &other) {
    v_ = other.v_;
    ++copy_calls_;
    return *this;
  }

  ProbeObject(ProbeObject &&) noexcept = default;
  ProbeObject &operator=(ProbeObject &&) noexcept = default;

  ~ProbeObject() { ++dtor_calls_; }

  static auto creations_count() { return ctor_calls_; }
  static auto destruction_count() { return dtor_calls_; }
  static auto copy_count() { return copy_calls_; }

private:
  static uint64_t ctor_calls_;
  static uint64_t dtor_calls_;
  static uint64_t copy_calls_;
  std::vector<double> v_;
};

uint64_t ProbeObject::ctor_calls_ = 0;
uint64_t ProbeObject::dtor_calls_ = 0;
uint64_t ProbeObject::copy_calls_ = 0;

TEST(AlignedStorageTest, Hard) {
  {
    static constexpr auto Align = 16UL;
    static constexpr auto Size = 32UL;

    auto value =
        make_storage<Align, Size, ProbeObject>(std::vector{3.14, 2.71, 9.81});
    const char dummy[] = {'S', 't', 'a', 'c', ' ', 'i', 's', ' ', 'c',
                          'o', 'r', 'r', 'u', 'p', 't', 'e', 'd'};

    //Check the object alignment
    ASSERT_EQ(reinterpret_cast<uintptr_t>(&(*value)) % Align, 0);
    //Check the object size
    std::vector<uint8_t> mem_dump(Size, 0);
    std::copy_n(reinterpret_cast<const uint8_t *>(&(*value)), Size,
                mem_dump.begin());
    for (auto i = sizeof(*value); i < mem_dump.size(); ++i)
      ASSERT_EQ(std::find(dummy, dummy + sizeof(dummy), mem_dump[i]),
                dummy + sizeof(dummy));

    // HW: Think how we can simplify check above.
    // HW: Make a suggestion how we can check that copy operation is correct.

    fmt::print("Memory dump: {:X}\n", fmt::join(mem_dump, ""));

    fmt::print("Storage address: {}\nObject address: {}\nStorage size: {}\n",
               static_cast<void *>(&value), static_cast<void *>(&(*value)),
               sizeof(value));
  }

  ASSERT_EQ(ProbeObject::copy_count(), 0);
  ASSERT_EQ(ProbeObject::creations_count(), 1);
  ASSERT_EQ(ProbeObject::destruction_count(), 1);
}

// HW: Make the same checks but with allocation on HEAP