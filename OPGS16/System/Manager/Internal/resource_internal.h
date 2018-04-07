#ifndef SYSTEM_MANAGER_INTERNAL_RESOURCE_INTERNAL_H
#define SYSTEM_MANAGER_INTERNAL_RESOURCE_INTERNAL_H

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
 * @file System/Manager/Internal/resource_internal.h
 * @author Jongmin Yun
 * @log
 * 2018-03-03 Create file.
 */

#include <string_view>  /*! std::string_view, operator""sv */

namespace opgs16 {
namespace resource {
namespace _internal {

using namespace std::string_view_literals;

constexpr std::string_view TEX2D{ "TEX2D"sv };
constexpr std::string_view TEX2DATLAS{ "TEX2DATLAS"sv };
constexpr std::string_view SHADE{ "SHADE"sv };
constexpr std::string_view SEBGM{ "SEBGM"sv };
constexpr std::string_view SEEFF{ "SEEFF"sv };
constexpr std::string_view FONT { "FONT"sv };
constexpr std::string_view ANIMATION{ "ANIMATION"sv };

enum class ESymbolType {
    NOTHING,                    /*! Default value of symbol type */
    RESOURCE,                   /*! Resource symbol. */
    GLOBAL_PATH,                /*! Global path symbol to read from. */
    COMMENT,                    /*! Comment symbol. */
};

enum class EResourceType {
    NOTHING,                    /*! Error type nothing found from token. */
    TEXTURE_2D,                 /*! Texture 2d */
    TEXTURE_2D_ATLAS,           /*! Texture 2d with atlas */
    SHADER,                     /*! Shader program */
    SOUND_EFFECT_BGM,           /*! SE background music */
    SOUND_EFFECT_EFFECT,        /*! SE effect sound */
    FONT,                       /*! Font file (.ttf) */
    ANIMATION,                  /*! Animation film */
};

} /*! opgs16::resource::_internal */
} /*! opgs16::resource */
} /*! opgs16 */

#endif // !SYSTEM_MANAGER_INTERNAL_RESOURCE_INTERNAL_H
