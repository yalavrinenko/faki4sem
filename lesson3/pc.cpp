#include "timer.h"
#include <fmt/core.h>
#include <future>
#include <queue>
#include <random>
#include <thread>
#include <vector>

auto generate(size_t N) {
  std::vector<double> v(N);
  static constexpr auto seed = 42;
  std::mt19937_64 eng(seed);

  std::uniform_real_distribution<double> distr(
      -std::numbers::pi_v<double>, std::numbers::pi_v<double>);

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

void EvalRanged(std::vector<double> &v, int from, int to) {
  for (auto i = from; i < to; ++i) { v[i] = F(v[i]); }
}

//using Queue = std::queue<std::pair<int, int>>;
class Queue {
public:
  void push(std::pair<int, int> q) { emplace(q); }

  template<typename... Args>
  void emplace(Args &&...args) {
    {
      std::lock_guard lg(mut_);
      q_.emplace(std::forward<Args>(args)...);
    }
    cv_.notify_all();
  }

  auto pop() {
    std::unique_lock lg(mut_);

    cv_.wait(lg, [this]() { return !q_.empty(); });

    auto T = q_.front();
    q_.pop();

    return T;
  }

private:
  std::queue<std::pair<int, int>> q_;
  std::condition_variable cv_;
  mutable std::mutex mut_;
};

inline constexpr auto end_task = std::pair<int, int>{-1, -1};

void consume(std::vector<double> &v, Queue &q) {
  bool is_end = false;
  while (!is_end) {
    auto [from, to] = q.pop();

    if (from == end_task.first && to == end_task.second) {
      is_end = true;
      q.push(end_task);
    } else {
      EvalRanged(v, from, to);
    }
  }
}

void producer(Queue &q, size_t size) {
  static constexpr auto task_size = 10000;
  for (auto i = 0U; i < size - task_size; i += task_size) {
    q.emplace(i, i + task_size);
  }
}

void pc(std::vector<double> &data, size_t NTHREADS) {
  Queue q;

  std::vector<std::future<void>> consumers;

  for (auto i = 0U; i < NTHREADS; ++i)
    consumers.emplace_back(std::async(std::launch::async, consume,
                                      std::ref(data), std::ref(q)));
  producer(q, data.size());

  q.emplace(end_task);

  for (auto &f: consumers) {
    try {
      f.get();
    } catch (std::exception &e) {
      fmt::print("Exception handled {}\n", e.what());
    }
  }
}

int main() {
  static constexpr auto size = 1'000'000;
  fmt::print("===Generate input ({} points)===\n", size);
  auto data = generate(size);
  fmt::print("===Start===\n");
  fmt::print("Single thread. Elapsed time = {} ms\n",
             timed_invoke(100, Eval, data));


  fmt::print("===Start Producer/Consumer===\n");
  for (auto threads_count: {2, 4, 8, 12, 16, 20, 32}) {
    fmt::print("Threads = {}. Elapsed time = {} ms\n", threads_count,
               timed_invoke(100, pc, data, threads_count));
  }

  return 0;
}