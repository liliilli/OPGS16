#ifndef OPGS16_SYSTEM_DEBUGS_LOGGER_H
#define OPGS16_SYSTEM_DEBUGS_LOGGER_H

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
 * @file System/Core/Public/logger.h
 * @brief Logger class declaration file.
 * @author Jongmin Yun
 * @log
 * 2018-03-06 Add logger frame.
 */

#include <atomic>
#include <list>
#include <mutex>
#include "../Internal/logger_internal.h"    /*! opgs16::debug::MsgTypeMSG */

namespace opgs16::debug {

/**
 * @class Logger
 * @brief Logger static sub-program.
 */
class Logger final {
public:
    using list_type = std::list<std::pair<MsgType, const char*>>;

public:
    /*! Get static instance of Logger. */
    static Logger& Instance() {
        /*! thread-safe magic static since C++11 */
		static Logger instance{};
		return instance;
	}

    void operator()() {
        while (m_is_loop) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_log.empty()) {
                Do(m_log.front());
                m_log.pop_front();
            }
        }
    }

    void Push(MsgType type, const char* message) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_log.emplace_back(type, message);
    }

    void Terminate() noexcept {
        m_is_loop = false;
    }

private:
    std::atomic<bool> m_is_loop {true};

    list_type   m_log{};
    std::mutex  m_mutex;

private:
    Logger() = default;

    void Do(const list_type::value_type& log);

public:
    Logger (const Logger&) = delete;
    Logger operator=(const Logger&) = delete;
};

inline void PushLog(const MsgType type, const char* log) {
    std::thread thread{ &Logger::Push, std::ref(Logger::Instance()), type, log };
    thread.detach();
}

} /*! opgs16::debug */

#endif /** OPGS16_SYSTEM_DEBUGS_LOGGER_H */
