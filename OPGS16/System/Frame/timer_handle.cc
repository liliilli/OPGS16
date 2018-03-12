#include "timer_handle.h"               /*! Header file */
#include "../Manager/Public/timer_manager.h"   /*! MTimerManager */

void TimerHandle::SetTimepoint(long interval) {
    m_interval  = static_cast<float>(interval) / 1000.f;
    m_remain    = m_interval;
}

void TimerHandle::SetCallbackFunction(std::function<void(void)> func) {
    m_callback  = func;
}

void TimerHandle::Try(const float quantum) {
    m_remain -= quantum;
    if (m_remain <= 0.f) {
        m_callback();   /*! Do it! */

        if (m_loop)     /*! If it has to loop, reset remain value */
            m_remain = m_interval;
        else {
            opgs16::manager::MTimerManager::Instance().DetachTimer(*this);
        }
    }
}