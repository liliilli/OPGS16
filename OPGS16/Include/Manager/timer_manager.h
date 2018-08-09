#ifndef OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/timer_manager.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-22 Implemented fundamental TimerManager class.
/// 2018-03-04 Refactoring.
/// 2018-06-16 Create file.
///

#include <functional>
#include <type_traits>
#include <unordered_map>

/// ::opgs16::manager::_internal::
#include <Manager/Internal/timer_internal.h>
/// ::opgs16::element::CTimerHandle
#include <Element/timer_handle.h>

//!
//! Forward declaration
//!

namespace opgs16::manager::timer {

using TTimerContainer = std::unordered_map<uint32_t, opgs16::manager::_internal::TimerWrapper>;
using TTimerCandidates = std::vector<TTimerContainer::value_type>;

} /// ::opgs16::manager::timer namespace

/// Internal namespace
namespace opgs16::manager::timer::__ {

///
/// @brief Return internal reference of timer container.
///
TTimerCandidates& Get() noexcept;

///
/// @brief Return internal timer id count.
///
uint32_t __GetTimerCount() noexcept;

///
/// @brief Set internal timer id count.
///
void __SetTimerCount(uint32_t value) noexcept;

} /// ::opgs16::manager::timer::__ namespace

///
/// @namespace timer
/// @brief Manager class to globally manage timers.
///
/// @todo Not precise timer because of dependency of tick fps delta time
///
namespace opgs16::manager::timer {

///
/// @brief Update tick count and check whether or not
/// any of timers was triggered.
///
/// @param[in] delta_time
///
void Update(float delta_time);

///
/// @brief Release all attached timers.
///
void ClearAllTimers();

///
/// @brief Pause timer.
/// If timer handler status is not Activated, do nothing and just return false.
/// @param[in] handle    TimerHandle to pause.
///
bool PauseTimer(element::CTimerHandle& handle);

///
/// @brief Resume paused timer.
/// If timer handler status is not Paused, do nothing and just return false.
/// @param[in] handle    TimerHandle to resume.
///
/// @todo Test it
///
bool ResumeTimer(element::CTimerHandle& handle);

///
/// @brief Deatch timer regardless of the status.
/// @param[in] handle    TimerHandle to resume.
///
bool DetachTimer(element::CTimerHandle& handle);

///
/// @brief Set Timer.
///
/// @param[in] handle    TimerHandle to bind.
/// @param[in] interval  Timer alarm interval. millisecond.
/// @param[in] is_loop   Loop mode variable. If this is true, this timer will be loop until
/// the object which     TimerHandle is being bound is destroyed.
/// @param[in] ref       Reference pointer related to callback function.
/// @param[in] func      Member function pointer or reference.
/// @param[in] args      Input arguments of member function.
///
template <
  class _Ref,
  class _Callable,
  class... _Argument,
  typename = std::enable_if_t<
  std::is_member_function_pointer_v<_Callable> &&
  std::is_same_v<void, std::result_of_t<_Callable && (_Ref, _Argument&&...)>>
  >
>
void SetTimer(element::CTimerHandle& handle, long interval, bool is_loop,
  _Ref* ref, _Callable&& func, _Argument&&... args) {
  // Setting
  handle.SetLoopMode(is_loop);
  handle.SetTimepoint(interval);
  const uint32_t count = __::__GetTimerCount();
  handle.SetKeyValue(count);

  const std::function<void()> callback = std::bind(std::forward<_Callable>(func),
      ref, std::forward<_Argument>(args)...);
  handle.SetCallbackFunction(callback);

  // Insert
  using _internal::TimerWrapper;
  using _internal::Status;

  __::Get().emplace_back(count, TimerWrapper(Status::ACTIVATED, &handle));
  __::__SetTimerCount(count + 1);
}

} /// ::opgs16::manager::timer namespace

#define OP16_TIMER_SET(__timer_ref__, __milli__, __loop__, __ref__, __func_ptr__) \
opgs16::manager::timer::SetTimer(__timer_ref__, __milli__, __loop__, __ref__, __func_ptr__)

#define OP16_TIMER_STOP(MATimerRef) \
opgs16::manager::timer::DetachTimer(MATimerRef)

#endif /// OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H
