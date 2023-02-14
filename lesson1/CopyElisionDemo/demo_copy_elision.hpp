#ifndef CMAKE_DEMO_DEMO_COPY_ELISION_HPP
#define CMAKE_DEMO_DEMO_COPY_ELISION_HPP

#include <cstdint>
#include <vector>

namespace lesson1::demo {
  class ProbeClass {
  public:
    ProbeClass();

    ProbeClass(const ProbeClass &);

    // Rule of 5.
    ProbeClass(ProbeClass &&) noexcept = default;
    ProbeClass &operator=(const ProbeClass &) = default;
    ProbeClass &operator=(ProbeClass &&) noexcept = default;

    ~ProbeClass();

    void update_state(uint64_t value) { values_.resize(value, value / 2); }

  private:
    std::vector<uint64_t> values_;
  };

  class CopyElision {
  public:
    ProbeClass operator()() const;
  };

  class CopyElisionFailed {
  public:
    const ProbeClass &operator()() const;
  };

  void show_copy_elision();

  void show_copy_elision_fail();
}// namespace lesson1::demo

#endif//CMAKE_DEMO_DEMO_COPY_ELISION_HPP
