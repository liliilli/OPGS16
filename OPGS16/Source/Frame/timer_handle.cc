/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Private/timer_handle.cc
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-22 Create file.
 * 2018-04-17 Move ::CTimerHandle a definition function body to ::opgs16::element::CTimerHandle.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Frame\timer_handle.h>     /// Header file
#include <Manager\timer_manager.h>  /// ::opgs16::manager::MTimerManager

namespace opgs16 {
namespace element {

void CTimerHandle::SetTimepoint(long interval) {
    m_interval  = static_cast<float>(interval) / 1000.f;
    m_remain    = m_interval;
}

void CTimerHandle::SetCallbackFunction(std::function<void(void)> func) {
    m_callback  = func;
}

void CTimerHandle::Try(const float quantum) {
    m_remain -= quantum;
    if (m_remain <= 0.f) {
        m_callback();   /*! Do it! */

        if (m_loop)     /*! If it has to loop, reset remain value */
            m_remain = m_interval;
        else {
            OP16_TIMER_STOP(*this);
        }
    }
}

} /*! opgs16::element */
} /*! opgs16 */
