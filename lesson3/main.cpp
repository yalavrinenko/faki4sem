//
// Created by yalavrinenko on 04.04.23.
//
#include "timer.h"
#include <fmt/core.h>
#include <future>
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

void MTEval(std::vector<double> &v, uint64_t NThreads) {
  std::vector<std::future<bool>> pool(NThreads);

  auto thread_func = [&v](uint64_t from, uint64_t to) {
    for (; from < to; ++from) v[from] = F(v[from]);
    return true;
  };

  auto start = 0ULL;
  auto size_per_thread = v.size() / NThreads;

  for (auto &thread: pool) {
    thread = std::async(std::launch::async, thread_func, start,
                        start + size_per_thread);
    start += size_per_thread;
  }

  for (auto &t: pool) {
    auto result = t.get();
    if (result == false) throw "Something gonna bad";
  }
}

int __main() {
  static constexpr auto size = 1'000'000;
  fmt::print("===Generate input ({} points)===\n", size);
  auto data = generate(size);
  fmt::print("===Start===\n");
  fmt::print("Single thread. Elapsed time = {} ms\n",
             timed_invoke(100, Eval, data));

  for (auto threads_count: {2, 4, 8, 12, 16, 20, 32}) {
    fmt::print("Threads = {}. Elapsed time = {} ms\n", threads_count,
               timed_invoke(100, MTEval, data, threads_count));
  }
  return 0;
}

std::atomic<int64_t> counter = 0;
std::mutex mut;

int main() {

  auto func_up = [](uint64_t n) {
    while (n > 0) {
      --n;
      counter++;
    }
  };

  auto func_down = [](uint64_t n) {
    while (n > 0) {
      --n;
      counter--;
    }
  };

  auto f1 = std::thread(func_up, 10000);
  auto f2 = std::thread(func_down, 10000);
  auto f3 = std::thread(func_up, 10000);
  auto f4 = std::thread(func_down, 10000);

  f1.join(), f2.join(), f3.join(), f4.join();
  fmt::print("{}\n", counter);

  return 0;
}