#ifndef OPGS16_SYSTEM_MANAGER_INTERNAL_TIMER_INTERNAL_H
#define OPGS16_SYSTEM_MANAGER_INTERNAL_TIMER_INTERNAL_H

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
 * @file System/Manager/Internal/timer_internal.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-04 Create file.
 */

#include "../../Frame/timer_handle.h"   /*! TimerHandle */

namespace opgs16 {
namespace manager {
namespace _internal {

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

    explicit TimerWrapper(const Status status, TimerHandle* handle) :
        m_status{ status }, m_handle{ handle } {};
    TimerWrapper(): m_status(Status::REMOVED) {} ;
};

} /*! opgs16::manager::_internal */
} /*! ogps16::manager */
} /*! opgs16 */


#endif // !OPGS16_SYSTEM_MANAGER_INTERNAL_TIMER_INTERNAL_H