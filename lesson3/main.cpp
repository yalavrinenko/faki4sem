//
// Created by yalavrinenko on 04.04.23.
//
#include "timer.h"
#include <fmt/core.h>
#include <math.h>
#include <numbers>
#include <random>
#include <thread>

double sqr(double x) { return x * x; }

auto generate(size_t N) {
  std::vector<double> v(N);
  static constexpr auto seed = 42;
  std::mt19937_64 eng(seed);

  std::uniform_real_distribution<double> distr(-std::numbers::pi_v<double>,
                                               std::numbers::pi_v<double>);

  std::generate_n(std::back_inserter(v), N,
                  [&distr, &eng]() { return distr(eng); });
  return v;
}

double F(double x) {
  double sum = 0;
  for (auto i = 0; i < 10; ++i)
    sum += std::tan(x + i * std::numbers::pi_v<double> / 3.0);
  return std::log(sum);
}

void Eval(std::vector<double> &v) {
  for (auto &e: v) { e = F(e); }
}

void MTEval(std::vector<double> &v) {
  static constexpr auto NThreads = 2;
  std::array<std::thread, NThreads> pool;

  auto thread_func = [&v](uint64_t from, uint64_t to) {
    for (; from < to; ++from) v[from] = F(v[from]);
  };

  auto start = 0ULL;
  auto size_per_thread = v.size() / NThreads;

  for (auto &thread: pool) {
    thread = std::thread(thread_func, start, start + size_per_thread);
    start += size_per_thread;
  }


}

int main() {
  static constexpr auto size = 1'000'000;
  fmt::print("===Generate input ({} points)===\n", size);
  auto data = generate(size);
  fmt::print("===Start===\n");
  fmt::print("Elapsed time = {} ms\n", timed_invoke(100, Eval, data));
  return 0;
}