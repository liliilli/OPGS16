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
 * @file System/Core/Private/logger.h
 * @brief CLogger class implementation file.
 * @author Jongmin Yun
 * @log
 * 2018-03-06 Add logger implementation file.
 * 2018-04-02 const char* to const wchar_t* for UTF-16 text.
 */

#include "../Public/logger.h"   /*! Header file */
#include "../Impl/logger_impl.h"    /*! ::opgs16::debug::_internal::CLoggerImpl */

namespace opgs16 {
namespace debug {

CLogger::CLogger() :
    m_impl { std::make_unique<_internal::CLoggerImpl>() } {

    auto& m_console = m_impl->Logger();
    using _internal::MsgType;
    Push(MsgType::INFO, L"Welcome to spdlog Hello world.");
    Push(MsgType::WARN, L"This is warning message...");
    Push(MsgType::_ERROR, L"An info message example 1..");
}

void CLogger::Push(_internal::MsgType type, const wchar_t* log_message) {
    using _internal::MsgType;
    switch (type) {
    case MsgType::INFO: m_impl->Logger().info(log_message);
        break;
    case MsgType::WARN: m_impl->Logger().warn(log_message);
        break;
    case MsgType::_ERROR: m_impl->Logger().error(log_message);
        break;
    default: /*! Do nothing */ break;
    }
}

void CLogger::Push(_internal::MsgType type, const char* log_message) {
    using _internal::MsgType;
    switch (type) {
    case MsgType::INFO: m_impl->Logger().info(log_message);
        break;
    case MsgType::WARN: m_impl->Logger().warn(log_message);
        break;
    case MsgType::_ERROR: m_impl->Logger().error(log_message);
        break;
    default: /*! Do nothing */ break;
    }
}

CLogger::~CLogger() = default;

void PushLog(_internal::MsgType type, const wchar_t* log_message) {
    CLogger::Instance().Push(type, log_message);
}

void PushLog(LOG_TYPE type, const wchar_t* log_message) {
    switch (type) {
    case LOG_TYPE_INFO: CLogger::Instance().Push(_internal::MsgType::INFO, log_message); break;
    case LOG_TYPE_WARN: CLogger::Instance().Push(_internal::MsgType::WARN, log_message); break;
    case LOG_TYPE_ERRO: CLogger::Instance().Push(_internal::MsgType::_ERROR, log_message); break;
    default: break;
    }
}

void PushLog(LOG_TYPE type, const char* log_message) {
    switch (type) {
    case LOG_TYPE_INFO: CLogger::Instance().Push(_internal::MsgType::INFO, log_message); break;
    case LOG_TYPE_WARN: CLogger::Instance().Push(_internal::MsgType::WARN, log_message); break;
    case LOG_TYPE_ERRO: CLogger::Instance().Push(_internal::MsgType::_ERROR, log_message); break;
    default: break;
    }
}

} /*! opgs16::debug */
} /*! opgs16 */
