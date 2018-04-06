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

#include "../Public/texture_builtin_default.h"      /*! Header file */

#include <fstream>      /*! std::ifstream */
#include <GL/glew.h>
#include "../../../../__ThirdParty/stb/stb_image.h" /*! stbi_image header only library */
#include "../../../../__ThirdParty/nlohmann/json.hpp" /*! json library */

#if defined(_DEBUG)
#include "../../../Core/Public/logger.h"
using opgs16::debug::PushLog;
using opgs16::debug::LOG_TYPE_INFO;
using opgs16::debug::LOG_TYPE_WARN;
using opgs16::debug::LOG_TYPE_ERRO;
#endif

namespace {
constexpr const char* system_sprite_texture_path = "Resources/Sprites/System.png";
constexpr const char* system_sprite_atlas_path = "Resources/Sprites/SystemAtlas.json";

GLenum ColorFormat(const int channels) noexcept {
switch (channels) {
    default: return GL_NONE;    /*! else, return Error type */
    case 1: return GL_RED;      /*! Gray or Red (one channel) */
    case 3: return GL_RGB;      /*! RGB no alpha */
    case 4: return GL_RGBA;     /*! RGB and alpha */
    }
}

void SetDefaultParameterSetting(CTexture2D& texture) {
    using element::texture::_internal::TextureParameter;
    std::vector<TextureParameter> t_p;  /*! Default Texture parameters */
    t_p.push_back(TextureParameter{ GL_TEXTURE_MIN_FILTER, GL_NEAREST });
    t_p.push_back(TextureParameter{ GL_TEXTURE_MAG_FILTER, GL_NEAREST });
    t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_S, GL_REPEAT });
    t_p.push_back(TextureParameter{ GL_TEXTURE_WRAP_T, GL_REPEAT });
    texture.SetTextureParameterI(t_p);
}
} /*! unnamed namespace */

namespace opgs16 {
namespace builtin {
namespace texture {

void GenerateAtlas();

void GenerateTexture() {
    stbi_set_flip_vertically_on_load(true);
    auto color_channels = 0;
    auto data = stbi_load(system_sprite_texture_path, &m_width, m_height, &color_channels, 0);
    if (!data) {
#if defined(_DEBUG)
        PushLog(LOG_TYPE_ERRO, L"Failed to create texture file.");
#endif
        return;
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    const auto color_format = ColorFormat(color_channels);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, m_width, m_height, 0, color_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //SetDefaultParameterSetting

}

void GenerateAtlas() {
    // does not yet support std::wstring.
    std::ifstream file{ system_sprite_atlas_path, std::ios_base::in };
#if defined(_DEBUG)
    if (file.bad()) {
        PushLog(LOG_TYPE_ERRO, "Failed to read atlas file.");
        throw std::runtime_error("Failed to read atlas file. system_sprite_atlas_path");
    }
#endif

    nlohmann::json json_container; file >> json_container;
    const auto texture_name     = json_container["texture_name"].dump();
    const auto fragment_indexes = json_container["index"].get<std::vector<int>>();

}

} /*! opgs16::builtin::texture */
} /*! opgs16::builtin */
} /*! opgs16 */
