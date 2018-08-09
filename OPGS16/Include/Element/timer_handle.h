#ifndef OPGS16_SYSTEM_FRAME_TIMER_HANDLE_H
#define OPGS16_SYSTEM_FRAME_TIMER_HANDLE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/timer_handle.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-22 Implemented fundamental TimerHandle class.
/// 2018-04-17 Move ::CTimerHandle to ::opgs16::element::CTimerHandle
///

#include <functional>

namespace opgs16::element {

///
/// @class CTimerHandle
/// @brief Handle instance manages each timer.
///
class CTimerHandle final {
public:
  /// Try callback execution.
  void Try(float quantum);

  /// Set looping mode.
  inline void SetLoopMode(bool value);

  /// Set hash_key value for TimerManager.
  inline void SetKeyValue(uint32_t key_value);

  /// Get hash_key value for TimerManager container.
  inline uint32_t GetKeyValue() const;

  ///
  /// @brief Set start, end time point have interval between them.
  /// @param[in] interval Interval of start and end timepoint.
  ///
  void SetTimepoint(long interval);

  /// Set callback function.
  void SetCallbackFunction(std::function<void(void)> func);

private:
  /// If this value is false, call callback once and destroy self.
  bool m_loop = false;
  /// Key value for accessing timer_manager container.
  uint32_t m_key_value = 0;

  float m_remain = 0;
  /// Timer interval.
  float m_interval = 0;

  /// Callback function
  std::function<void(void)> m_callback;
};

inline void CTimerHandle::SetLoopMode(bool value) {
  m_loop = value;
}

inline void CTimerHandle::SetKeyValue(uint32_t key_value) {
  m_key_value = key_value;
}

inline uint32_t CTimerHandle::GetKeyValue() const {
  return m_key_value;
}

} /// ::opgs16::element namespace

#endif /// !OPGS16_SYSTEM_FRAME_TIMER_HANDLE_H