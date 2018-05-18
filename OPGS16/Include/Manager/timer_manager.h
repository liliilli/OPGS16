#ifndef OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Manager/Public/timer_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-22 Implemented fundamental TimerManager class.
 * 2018-03-04 Refactoring.
 */

#include <algorithm>                        /*! std::hash */
#include <functional>                       /*! std::functional */
#include <limits>                           /*! std::limit<size_t> */
#include <type_traits>
#include <unordered_map>                    /*! std::unordered_map */

#include "../Internal/timer_internal.h"     /*! _internal:: */
#include "../../Frame/timer_handle.h"       /*! TimerHandle Definition */

namespace opgs16 {
namespace manager {

/*!
 * @class MTimerManager
 * @brief Manager class to globally manage timers.
 * @caveat Not precise timer because of dependency of tick delta time (fps tick interval)
 */
class MTimerManager final {
public:
    /*!
     * @brief Get reference of unique instance of TimerManager.
     * @return Reference instance of timer manager.
     */
    static MTimerManager& Instance() {
        static MTimerManager instance{};
        return instance;
    }

    /*! Update tick count and check whether or not any of timers was triggered. */
    void Update();

    /*! Clean all attached timers. */
    void Clear();

    /*!
     * @brief Set Timer.
     *
     * @param[in] handle    TimerHandle to bind.
     * @param[in] interval  Timer alarm interval. millisecond.
     * @param[in] is_loop   Loop mode variable. If this is true, this timer will be loop until
     * the object which     TimerHandle is being bound is destroyed.
     * @param[in] ref       Reference pointer related to callback function.
     * @param[in] func      Member function pointer or reference.
     * @param[in] args      Input arguments of member function.
     */
    template <
        class _Ref,
        class _Callable,
        class... _Argument,
        typename = std::enable_if_t<
            std::is_member_function_pointer_v<_Callable> &&
            std::is_same_v<void, std::result_of_t<_Callable&&(_Ref, _Argument&&...)>>
            >
        >
    void SetTimer(element::CTimerHandle& handle, long interval, bool is_loop,
                  _Ref* ref, _Callable&& func, _Argument&&... args);

    /*!
     * @brief Pause timer.
     * If timer handler status is not Activated, do nothing and just return false.
     * @param[in] handle    TimerHandle to pause.
     */
    bool PauseTimer(element::CTimerHandle& handle);

    /*!
     * @brief Resume paused timer.
     * If timer handler status is not Paused, do nothing and just return false.
     * @param[in] handle    TimerHandle to resume.
     * @todo Test it
     */
    bool ResumeTimer(element::CTimerHandle& handle);

    /*!
     * @brief Deatch timer regardless of the status.
     * @param[in] handle    TimerHandle to resume.
     */
    bool DetachTimer(element::CTimerHandle& handle);

private:
    /*! key value for assignment of timer handler, default is 0 */
    inline static size_t s_timer_count = 0;

    std::unordered_map<size_t, _internal::TimerWrapper> m_timer_container; /*! Timer container */
    std::list<size_t> m_delete_list; /*! Deletion candidates list */

private:
    MTimerManager() = default;

    inline bool DoesTimerExist(const element::CTimerHandle& handle) {
        return m_timer_container.find(handle.GetKeyValue()) != m_timer_container.end();
    }

public:
    MTimerManager(const MTimerManager&) = delete;
    MTimerManager& operator=(const MTimerManager&) = delete;
};

template <
    class _Ref,
    class _Callable,
    class... _Argument,
    typename
>
void MTimerManager::SetTimer(element::CTimerHandle& handle, long interval, bool is_loop,
                            _Ref* ref, _Callable&& func, _Argument&&... args) {
    /*! Setting */
    handle.SetLoopMode(is_loop);
    handle.SetTimepoint(interval);
    handle.SetKeyValue(s_timer_count);

    std::function<void(void)> i = std::bind(std::forward<_Callable>(func),
                                            ref,
                                            std::forward<_Argument>(args)...);
    handle.SetCallbackFunction(i);

    /*! Insert */
    using _internal::TimerWrapper;
    using _internal::Status;
    m_timer_container.emplace(std::make_pair(s_timer_count,
                                             TimerWrapper( Status::ACTIVATED, &handle ))
    );

    s_timer_count += 1;

    /*! Check s_timer_count is about to being overflowed */
    if (s_timer_count == std::numeric_limits<size_t>::max())
        s_timer_count = std::numeric_limits<size_t>::min();
}

} /*! opgs16::manager */
} /*! opgs16 */

#define M_SET_TIMER(__timer_ref__, __milli__, __loop__, __ref__, __func_ptr__) \
opgs16::manager::MTimerManager::Instance().SetTimer(__timer_ref__, __milli__, __loop__, __ref__, __func_ptr__)

#endif // !OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H
