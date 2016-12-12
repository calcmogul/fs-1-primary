// Copyright (c) 2016-2017 Formula Slug. All Rights Reserved.

#pragma once

#include <stdint.h>

#include <utility>

#include "hal.h"

template <typename Function, typename... Args>
thread<Function, Args...>::thread(thread&& rhs) noexcept {
  m_thread = rhs.m_thread;
  m_joinable = rhs.m_joinable.load();
  rhs.m_thread = nullptr;
  rhs.m_joinable = false;
}

/**
 * Create and launch a thread.
 *
 * @param function The address of the function to run as the new task.
 * @param args A parameter pack of arguments to pass to the function.
 */
template <typename Function, typename... Args>
thread<Function, Args...>::thread(tprio_t priority, Function&& func,
                                  Args&&... args) {
  m_func = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);

  const auto threadFunc = [](void* arg) {
    auto state = reinterpret_cast<decltype(m_func)*>(arg);
    (*state)();
  };

  m_thread = chThdCreateStatic(m_workingArea,
                               sizeof(m_workingArea),  // Stack size
                               priority, threadFunc, &m_func);
  if (m_thread == nullptr) {
    chSysHalt("Out of memory");
  }

  m_joinable = true;
}

template <typename Function, typename... Args>
thread<Function, Args...>& thread<Function, Args...>::operator=(
    thread&& rhs) noexcept {
  m_thread = rhs.m_thread;
  m_joinable = rhs.m_joinable.load();
  rhs.m_thread = nullptr;
  rhs.m_joinable = false;

  return *this;
}

template <typename Function, typename... Args>
thread<Function, Args...>::~thread() {
  chThdRelease(m_thread);
  if (m_joinable) {
    m_joinable = false;
  }
}

template <typename Function, typename... Args>
bool thread<Function, Args...>::joinable() const noexcept {
  return m_joinable;
}

template <typename Function, typename... Args>
void thread<Function, Args...>::join() {
  chThdWait(m_thread);
}

template <typename Function, typename... Args>
void thread<Function, Args...>::detach() {
  m_joinable = false;
}

template <typename Function, typename... Args>
typename thread<Function, Args...>::id thread<Function, Args...>::get_id() const
    noexcept {
  return m_thread;
}

template <typename Function, typename... Args>
typename thread<Function, Args...>::native_handle_type
thread<Function, Args...>::native_handle() {
  return m_thread;
}

template <typename Function, typename... Args>
thread<Function, Args...> make_thread(tprio_t priority, Function&& func,
                                      Args&&... args) {
  return thread<Function, Args...>(priority, std::forward<Function>(func),
                                   std::forward<Args>(args)...);
}
