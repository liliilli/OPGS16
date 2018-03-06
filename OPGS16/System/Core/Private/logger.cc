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
 * @brief Logger class implementation file.
 * @author Jongmin Yun
 * @log
 * 2018-03-06 Add logger implementation file.
 */

#include "../Public/logger.h"   /*! Header file */
#include <cstdio>
#include <ctime>

#include <chrono>
#include <sstream>
#include <iomanip>

namespace opgs16 {
namespace debug {

void Logger::Do(const list_type::value_type& log) {
    using std::chrono::system_clock;

    auto&[type, message] = log;
    const auto time_struct = system_clock::to_time_t(system_clock::now());

    std::ostringstream stream;
    stream << std::put_time(std::localtime(&time_struct), "%F %T");

    switch (type) {
    case MsgType::OK:       stream << "    OK : "; break;
    case MsgType::ERROR:    stream << " ERROR : "; break;
    }

    stream << message;
    printf("%s\n", stream.str().c_str());
}

} /*! opgs16::debug */
} /*! opgs16 */
