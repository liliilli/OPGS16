#ifndef OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H

/*!
 * @file System\Manager\timer_manager.h
 * @author Jongmin Yun
 * @date 2018-02-22
 *
 * @log
 * 2018-02-22 Implemented fundamental TimerManager class.
 */

#include <algorithm>                        /*! std::hash */
#include <chrono>                           /*! std::chrono */
#include <functional>                       /*! std::functional */
#include <limits>                           /*! std::limit<size_t> */
#include <unordered_map>                    /*! std::unordered_map */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! Object
                                              * TimerHandle */
#include "..\Frame\timer_handle.h"          /*! TimerHandle Definition */

/*!
 * @class TimerManager
 * @brief Manager class to globally manage timers.
 * @caveat Not precise timer because of dependency of tick delta time (fps tick interval)
 */
class TimerManager final {
private:
    /*! This status enum is used for notifying status information of each timer handler. */
    enum class Status {
        ACTIVATED,  /*! Timer handler is activated */
        PAUSED,     /*! Timer handler is paused */
        REMOVED,    /*! Timer handler will be removed */
    };

    /*! This structure is used to store handler pointer and status of timer handler. */
    struct TimerWrapper {
        Status m_status;
        TimerHandle* m_handle{ nullptr };

        explicit TimerWrapper(Status status, TimerHandle* handle) :
            m_status{ status }, m_handle{ handle } {};
    };

public:
    /*!
     * @brief Get reference of unique instance of TimerManager.
     * @return Reference instance of timer manager.
     */
    static TimerManager& GetInstance() {
        static TimerManager instance{};
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
     * @param[in] pmf       Member function pointer or reference.
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
    void SetTimer(TimerHandle& handle, long interval, bool is_loop,
                  _Ref* ref, _Callable&& pmf, _Argument&&... args);

    /*!
     * @brief Pause timer.
     * If timer handler status is not Activated, do nothing and just return false.
     * @param[in] handle    TimerHandle to pause.
     */
    bool PauseTimer(TimerHandle& handle);

    /*!
     * @brief Resume paused timer.
     * If timer handler status is not Paused, do nothing and just return false.
     * @param[in] handle    TimerHandle to resume.
     * @todo Test it
     */
    bool ResumeTimer(TimerHandle& handle);

    /*!
     * @brief Deatch timer regardless of the status.
     * @param[in] handle    TimerHandle to resume.
     * @todo Test it
     */
    bool DetachTimer(TimerHandle& handle);

private:
    static size_t s_timer_count;    /*! key value for assignment of timer handler, default is 0 */

    std::unordered_map<size_t, TimerWrapper> m_timer_container; /*! Timer container */
    std::list<size_t> m_delete_list; /*! Deletion candidates list */

private:
    TimerManager() = default;
    ~TimerManager() = default;
    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;
};

template <class _Ref, class _Callable, class... _Argument, typename>
void TimerManager::SetTimer(TimerHandle& handle, long interval, bool is_loop,
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
    m_timer_container.emplace(std::make_pair(s_timer_count,
                                             TimerWrapper( Status::ACTIVATED, &handle ))
    );

    s_timer_count += 1;

    /*! Check s_timer_count is about to being overflowed */
    if (s_timer_count == std::numeric_limits<size_t>::max())
        s_timer_count = std::numeric_limits<size_t>::min();
}

#endif // !OPGS16_SYSTEM_MANAGER_TIMER_MANAGER_H
