//
// Created by yalavrinenko on 04.04.23.
//

#ifndef LESSON3_TIMER_H
#define LESSON3_TIMER_H

#include <chrono>
#include <functional>
#include <utility>

using Time = std::chrono::milliseconds;

template<typename Func, typename... Args>
double timed_invoke(size_t repeat, Func &&func, Args &&...args) {
  auto stime = std::chrono::high_resolution_clock::now();
  for (auto i = 0U; i < repeat; ++i)
    std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
  auto ftime = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<Time>(ftime - stime).count() /
         static_cast<double>(repeat);
}

#endif//LESSON3_TIMER_H
