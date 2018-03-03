#ifndef OPGS16_SYSTEM_MANAGER_PUBLIC_RESOURCE_TYPE_H
#define OPGS16_SYSTEM_MANAGER_PUBLIC_RESOURCE_TYPE_H

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
 * @file System/Manager/Public/resource_type.h
 * @brief Resource information structures.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-02 Refactored. Create SFont (Struct_Font) concrete type stores font path infos.
 * 2018-03-03 Add Sound, Shader type and enumration used in everywhere.
 */

#include <list>         /*! std::list */
#include <string>       /*! std::string */

#include <GL\glew.h>
#include <glm\glm.hpp>  /*! glm vector types */

namespace opgs16 {
namespace resource {

/*! Resource scope type */
enum class EScopeType {
    GLOBAL,     /*! G */
    SCENE_LOCAL /*! L */
};

EScopeType GetScopeType(char type);

/*! Information structure of Texture2D */
struct Texture2D {
    struct IndexSize { unsigned x_sep, y_sep; };

    EScopeType       m_type;
    std::string     m_path;
    IndexSize       m_nm_size;
};

/*! Information structure of Font file */
class SFont {
    EScopeType       m_type;
    std::string     m_path;
    bool            m_default_font;

public:
    explicit SFont(EScopeType type, std::string path, bool default_font) :
        m_type{ type }, m_path{ path }, m_default_font{ default_font } {};
    inline EScopeType Type() const noexcept {
        return m_type;
    }
    inline std::string Path() const noexcept {
        return m_path;
    }
    inline bool IsDefault() const noexcept {
        return m_default_font;
    }
};

class SSound {
    EScopeType       m_type;
    std::string     m_path;
    bool            m_is_bgm;

public:
    explicit SSound(EScopeType type, std::string path, bool is_bgm) :
        m_type{ type }, m_path{ path }, m_is_bgm{ is_bgm } {};
    inline EScopeType Type() const noexcept { return m_type; }
    inline std::string Path() const noexcept { return m_path; }
    inline bool IsBgm() const noexcept { return m_is_bgm; }
};

enum class EShaderType : int {
    VS  = GL_VERTEX_SHADER,
    TCS = GL_TESS_CONTROL_SHADER,
    TES = GL_TESS_EVALUATION_SHADER,
    GS  = GL_GEOMETRY_SHADER,
    FS  = GL_FRAGMENT_SHADER
};

inline EShaderType GetShaderType(const std::string& token) {
    if (token == "VS")          return EShaderType::VS;
    else if (token == "GS")     return EShaderType::GS;
    else if (token == "TCS")    return EShaderType::TCS;
    else if (token == "TES")    return EShaderType::TES;
    else if (token == "FS")     return EShaderType::FS;
}

class SShader {
public:
    using shader_pair = std::pair<EShaderType, std::string>;
    using shader_list = std::list<shader_pair>;

private:
    EScopeType       m_type;
    shader_list     m_shader_list;

public:
    explicit SShader(EScopeType type, shader_list list) :
        m_type{ type }, m_shader_list{ list } {};
    inline EScopeType Type() const noexcept {
        return m_type;
    }
    inline const shader_list& List() const noexcept {
        return m_shader_list;
    }
};

} /*! opgs16::resource */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_PUBLIC_RESOURCE_TYPE_H

