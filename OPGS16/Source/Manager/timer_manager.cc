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
 * @file System/Manager/Private/timer_manager.cc
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-22 Implemented fundamental TimerManager class.
 * 2018-03-04 Refactoring.
 */

#include <Manager\timer_manager.h>  /// Header file
#include <Manager\time_manager.h>   /// ::opgs16::manager::MTimeManager

namespace opgs16::manager {

using _internal::Status;

void MTimerManager::Update() {
    /*! Iteration & Checking */
    const auto time_quantum = MTimeManager::Instance().GetDeltaTime();
    for (auto& [__, timer] : m_timer_container) {
        if (timer.m_status == Status::ACTIVATED)
            timer.m_handle->Try(time_quantum);
    }

    /*! Deletion */
    for (const auto& keyval : m_delete_list) {
        m_timer_container.erase(keyval);
    }
}

void MTimerManager::Clear() {
    s_timer_count = 0u;
    m_delete_list.clear();
    m_timer_container.clear();
}

bool MTimerManager::PauseTimer(element::CTimerHandle& handle) {
    if (const auto key = handle.GetKeyValue();
        DoesTimerExist(handle) && m_timer_container[key].m_status == Status::ACTIVATED) {

        m_timer_container.at(key).m_status = Status::PAUSED;
        return true;
    }
    return false;
}

bool MTimerManager::ResumeTimer(element::CTimerHandle& handle) {
    if (const auto key = handle.GetKeyValue();
        DoesTimerExist(handle) && m_timer_container[key].m_status == Status::PAUSED) {

        m_timer_container[key].m_status = Status::ACTIVATED;
        return true;
    }
    return false;
}

bool MTimerManager::DetachTimer(element::CTimerHandle& handle) {
    if (const auto key = handle.GetKeyValue(); DoesTimerExist(handle)) {

        m_timer_container[key].m_status = Status::REMOVED;
        m_delete_list.emplace_back(key);
        return true;
    }
    return false;
}

} /*! opgs16::manager */
