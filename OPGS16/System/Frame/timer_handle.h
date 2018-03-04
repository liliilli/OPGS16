#ifndef OPGS16_SYSTEM_FRAME_TIMER_HANDLE_H
#define OPGS16_SYSTEM_FRAME_TIMER_HANDLE_H

/*!
 * @file System\Manager\timer_handle.h
 * @author Jongmin Yun
 * @date 2018-02-22
 *
 * @log
 * 2018-02-22 Implemented fundamental TimerHandle class.
 */

#include <chrono>       /*! std::chrono */
#include <functional>   /*! std::bind */
#include <thread>       /*! std::thread */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! Object */

/*!
 * @class TimerHandle
 * @brief Handle instance manages each timer.
 */
class TimerHandle final {
public:
    /*! Try callback execution */
    void Try(const float quantum);

    /*! Set looping mode */
    inline void SetLoopMode(bool value);

    /*! Set hash_key value for TimerManager */
    inline void SetKeyValue(size_t key);

    /*! Get hash_key value for TimerManager container */
    inline size_t GetKeyValue() const;

    /*!
     * @brief Set start, end time point have interval between them.
     * @param[in] interval Interval of start and end timepoint.
     */
    void SetTimepoint(long interval);

    /*! Set callback function */
    void SetCallbackFunction(std::function<void(void)> func);

private:
    bool m_loop{ false };       /*! If this value is false, call callback once and destroy self. */
    size_t m_key_value;         /*! Key value for accessing timer_manager container */

    float m_remain;             /*! Remained time count */
    float m_interval;           /*! Timer interval */

    std::function<void(void)> m_callback;   /*! Callback function */
};

inline void TimerHandle::SetLoopMode(bool value) {
    m_loop = value;
}

inline void TimerHandle::SetKeyValue(size_t value) {
    m_key_value = value;
}

inline size_t TimerHandle::GetKeyValue() const {
    return m_key_value;
}

#endif // !OPGS16_SYSTEM_FRAME_TIMER_HANDLE_H