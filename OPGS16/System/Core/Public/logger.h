#ifndef OPGS16_SYSTEM_CORE_PUBLIC_LOGGER_H
#define OPGS16_SYSTEM_CORE_PUBLIC_LOGGER_H

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
 * @brief CLogger class declaration file.
 * @author Jongmin Yun
 * @log
 * 2018-03-06 Add logger frame.
 * 2018-03-11 Attach spdlog logging library.
 * 2018-04-02 const char* to const wchar_t* for multibyte text.
 */

#include <memory>
#include "../Internal/logger_internal.h"    /*! ::opgs16::debug::_internal */
#include "../../../Headers/Fwd/objectfwd.h" /*! Forward declaration */
typedef int LOG_TYPE;

namespace opgs16 {
namespace debug {

constexpr int LOG_TYPE_INFO = 0;
constexpr int LOG_TYPE_WARN = 1;
constexpr int LOG_TYPE_ERRO = 2;

/**
 * @class CLogger
 * @brief CLogger static sub-program.
 */
class CLogger final {
public:
    /*! Get static instance of CLogger. */
    static CLogger& Instance() {
        static CLogger instance{};
        return instance;
    }

    /*! Push message into logger */
    void Push(_internal::MsgType type, const wchar_t* log_message);
    void Push(_internal::MsgType type, const char* log_message);

private:
    CLogger();
    std::unique_ptr<_internal::CLoggerImpl> m_impl; /*! Private implementation instance. */

public:
    ~CLogger();
    CLogger(const CLogger&) = delete;
    CLogger operator=(const CLogger&) = delete;
};

/*! Helper function */
void PushLog(_internal::MsgType type, const wchar_t* log_message);
void PushLog(LOG_TYPE type, const wchar_t* log_message);
void PushLog(LOG_TYPE type, const char* log_message);

} /*! opgs16::debug */
} /*! opgs16 */

#endif /** OPGS16_SYSTEM_CORE_PUBLIC_LOGGER_H */
