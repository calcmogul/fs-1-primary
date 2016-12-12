// Copyright (c) 2016-2017 Formula Slug. All Rights Reserved.

#pragma once

#include <atomic>
#include <functional>

#include "ch.hpp"

/**
 * Wrapper class around ChibiOS dynamic threads
 */
template <typename Function, typename... Args>
class thread {
 public:
  thread() = default;
  thread(thread&& rhs) noexcept;
  thread(const thread&) = delete;
  thread& operator=(const thread&) = delete;
  thread& operator=(thread&& rhs) noexcept;

  thread(tprio_t priority, Function&& func, Args&&... args);

  virtual ~thread();

  using id = thread_t*;
  using native_handle_type = thread_t*;

  bool joinable() const noexcept;
  void join();
  void detach();
  thread::id get_id() const noexcept;
  thread::native_handle_type native_handle();

 private:
  thread_t* m_thread;
  THD_WORKING_AREA(m_workingArea, 128);
  using Ret = typename std::result_of<Function&(Args...)>::type;
  std::function<Ret()> m_func;
  std::atomic<bool> m_joinable{false};
};

template <typename Function, typename... Args>
thread<Function, Args...> make_thread(tprio_t priority, Function&& func,
                                      Args&&... args);

#include "thread.inc"
