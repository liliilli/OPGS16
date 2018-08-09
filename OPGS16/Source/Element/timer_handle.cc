#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/timer_handle.cc
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-22 Create file.
/// 2018-04-17 Move ::CTimerHandle a definition function body to ::opgs16::element::CTimerHandle.
///

/// Header file
#include <Element/timer_handle.h>
#include <Manager/timer_manager.h>

namespace opgs16::element {

void CTimerHandle::SetTimepoint(long interval) {
  m_interval = static_cast<float>(interval) / 1000.f;
  m_remain = m_interval;
}

void CTimerHandle::SetCallbackFunction(std::function<void(void)> func) {
  m_callback = func;
}

void CTimerHandle::Try(float quantum) {
  m_remain -= quantum;
  if (m_remain <= 0.f) {
    // Do it!
    m_callback();

    // If it has to loop, reset remain value
    if (m_loop)
      m_remain = m_interval;
    else {
      OP16_TIMER_STOP(*this);
    }
  }
}

} /// ::opgs16::element namespace
