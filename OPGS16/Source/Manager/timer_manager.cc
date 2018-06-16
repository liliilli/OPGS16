
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/timer_manager.cc
/// @author Jongmin Yun
///
/// @log
/// 2018-02-22 Implemented fundamental TimerManager class.
/// 2018-03-04 Refactoring.
/// 2018-06-15 Refactoring.
///

/// Header file
#include <Manager/timer_manager.h>

#include <limits>

/// import logger for debug mode.
#include <Headers/import_logger.h>

//!
//! Data
//!

namespace {

// key value for assignment of timer handler, default is 0
uint32_t s_timer_count = 0;

constexpr uint32_t s_rehash_init_value = 100;
uint32_t s_rehash_count = s_rehash_init_value;

// Timer container
opgs16::manager::timer::TTimerContainer m_timer_container;

// Deletion candidates list
std::list<uint32_t> m_delete_list;

} /// unnamed namespace

//!
//! Global functions
//!

namespace {

///
/// @brief
///
bool IsTimerExist(const opgs16::element::CTimerHandle& handle) {
  return m_timer_container.find(handle.GetKeyValue()) != m_timer_container.end();
}

} /// unnamed namespace

//!
//! Implementation.
//!

namespace opgs16::manager::timer {

TTimerContainer& __::Get() noexcept {
  return m_timer_container;
}

uint32_t __::__GetTimerCount() noexcept {
  return s_timer_count;
}

void __::__SetTimerCount(uint32_t value) noexcept {
  s_timer_count = value;

  // Check s_timer_count is about to being overflowed
  if (s_timer_count == std::numeric_limits<uint32_t>::max())
    s_timer_count = std::numeric_limits<uint32_t>::min();
}

  using _internal::Status;

void Update(float delta_time) {
  // Iteration & Timer Check
  for (auto&[__, timer] : m_timer_container) {
    if (timer.m_status == Status::ACTIVATED)
      timer.m_handle->Try(delta_time);
  }

  // Deletion
  for (const auto& keyval : m_delete_list) {
    m_timer_container.erase(keyval);
    --s_rehash_count;
  }
  m_delete_list.clear();

  if (s_rehash_count <= 0) {
    s_rehash_count = s_rehash_init_value;
    m_timer_container.rehash(m_timer_container.size());
    PUSH_LOG_CRITICAL("Timer table rehashing");
  }
}

void ClearAllTimers() {
  s_timer_count = 0u;
  m_delete_list.clear();
  m_timer_container.clear();
}

bool PauseTimer(element::CTimerHandle& handle) {
  if (const auto key = handle.GetKeyValue();
  IsTimerExist(handle) && m_timer_container[key].m_status == Status::ACTIVATED) {

    m_timer_container.at(key).m_status = Status::PAUSED;
    return true;
  }
  return false;
}

bool ResumeTimer(element::CTimerHandle& handle) {
  if (const auto key = handle.GetKeyValue();
  IsTimerExist(handle) && m_timer_container[key].m_status == Status::PAUSED) {

    m_timer_container[key].m_status = Status::ACTIVATED;
    return true;
  }
  return false;
}

bool DetachTimer(element::CTimerHandle& handle) {
  if (const auto key = handle.GetKeyValue(); IsTimerExist(handle)) {

    m_timer_container[key].m_status = Status::REMOVED;
    m_delete_list.emplace_back(key);
    return true;
  }
  return false;
}

} /// ::opgs16::manager::timer namespace
