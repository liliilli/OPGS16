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
 * @file System/Manager/Private/font_manager.cc
 * @brief
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-03 Refactoring.
 */

#include "font_manager.h"   /*! Header file */
#include <functional>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>

#include "Public/resource_manager.h"
#include "../Core/Public/core_setting.h"
#include "../../System/Shader/shader_manager.h"

namespace opgs16 {
namespace manager {

FontManager::FontManager() :
    m_projection{ glm::ortho(0.f, static_cast<float>(GlobalSetting::ScreenWidth()),
                             0.f, static_cast<float>(GlobalSetting::ScreenHeight())) } {
	m_shader = ShaderManager::GetInstance().GetShaderWithName("gCommonFont");
	BindVertexAttributes();
}

void FontManager::BindVertexAttributes() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool FontManager::GenerateFont(const std::string& name_tag) {
    if (DoesFontExist(name_tag))
        return false;

    const auto [success, information] = ResourceManager::Instance().GetFont(name_tag);
    if (success && CheckFreeType() && LoadFreeType(information->Path())) {
		FT_Set_Pixel_Sizes(m_ft_face, 0, k_default_font_size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Create font and move it.
		m_fonts.emplace(name_tag, GetCharTextures());
		// If caller order this font must be a default, insert raw pointer.
		if (information->IsDefault())
            m_default_font = m_fonts.at(name_tag).get();

		FT_Done_Face(m_ft_face);
		FT_Done_FreeType(m_freetype);
		return true;
    }
    else return false;
}

FontManager::font_map_ptr FontManager::GetCharTextures() const {
	auto glyphs = std::make_unique<font_type>();

    for (GLubyte c = 0; c < 128; ++c) {
        if (FT_Load_Char(m_ft_face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph\n";
            continue;
        }

        /*! Generate Texture */
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        const auto width  = m_ft_face->glyph->bitmap.width;
        const auto height = m_ft_face->glyph->bitmap.rows;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     width, height,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     m_ft_face->glyph->bitmap.buffer);

        // Set Texture Options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Store character for later use
        glyphs->emplace(c, Character{ texture,
                        glm::ivec2(width, height),
                        glm::ivec2(m_ft_face->glyph->bitmap_left, m_ft_face->glyph->bitmap_top),
                        static_cast<GLuint>(m_ft_face->glyph->advance.x)
                        });
    }

	return glyphs;
}

bool FontManager::LoadDefaultFont() {
	if (m_default_font == nullptr)
        return false;
    else {
        m_font_in_use = m_default_font;
        return true;
    }
}

bool FontManager::LoadFont(const std::string& tag) {
    if (DoesFontExist(tag)) {
        m_font_in_use = m_fonts[tag].get();
        return true;
    }
    else return false;
}

bool FontManager::DeleteFont(const std::string& tag) {
    if (DoesFontExist(tag)) {
        /** Remove pointer reference */
        auto font = m_fonts.at(tag).get();
        if (m_font_in_use == font) m_font_in_use = nullptr;
        if (m_default_font == font) m_default_font = nullptr;

        m_fonts.erase(tag);
        return true;
    }
    else return false;
}

void FontManager::RenderTextNew (const std::string& text,
                                 IOriginable::Origin origin,
                                 const glm::vec2 final_position,
                                 const glm::vec3 color,
                                 IAlignable::Alignment alignment,
                                 const float scale) {
    if (m_font_in_use) {
        StartShader(color);

        std::vector<std::string> text_container;
        if (std::string::npos == text.find('\n'))
            text_container.emplace_back(text);
        else
            text_container = SeparateTextToList(text);

        switch (typedef IAlignable::Alignment Align; alignment) {
        case Align::LEFT:   RenderLeftSide(text_container, final_position, scale);      break;
        case Align::CENTER: RenderCenterSide(text_container, final_position, scale);    break;
        case Align::RIGHT:  RenderRightSide(text_container, final_position, scale);     break;
        }

        EndShader();
    }
}

std::vector<std::string> FontManager::SeparateTextToList(const std::string& text) const {
    std::vector<std::string> result;

    std::stringstream stream{text};
    for (std::string line; std::getline(stream, line);)
        result.emplace_back(line);

    return result;
}

void FontManager::StartShader(const glm::vec3& color) const {
	m_shader->Use();
	m_shader->SetVec3f("textColor", color);
	m_shader->SetVecMatrix4f("projection", m_projection);
	glBindVertexArray(m_vao);
}

void FontManager::RenderLeftSide(const std::vector<std::string>& container,
                                 const glm::vec2& position,
                                 const float scale) const {
	auto pos = position;

	for (const auto& t_text : container) {
		for (const auto& chr : t_text) {
		    const Character ch_info = (*m_font_in_use)[chr];
			Render(ch_info, GetCharacterVertices(ch_info, pos, scale));
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= (*m_font_in_use)[0].size.y * 1.5f;
	}
}

void FontManager::RenderCenterSide(const std::vector<std::string>& container,
                                   const glm::vec2& position,
                                   const float scale) const {
	auto pos = position;

	for (const auto& t_text : container) {
		pos.x -= GetStringRenderWidth(t_text, scale) >> 1;

		for (const auto& chr : t_text) {
			Character ch_info = (*m_font_in_use)[chr];
			Render(ch_info, GetCharacterVertices(ch_info, pos, scale));
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= (*m_font_in_use)[0].size.y * 1.5f;
	}
}

void FontManager::RenderRightSide(const std::vector<std::string>& container,
                                  const glm::vec2& position,
                                  const float scale) const {
	auto pos = position;

	for (const auto& t_text : container) {
		pos.x -= GetStringRenderWidth(t_text, scale);

		for (const auto& chr : t_text) {
			Character ch_info = (*m_font_in_use)[chr];
			Render(ch_info, GetCharacterVertices(ch_info, pos, scale));
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= (*m_font_in_use)[0].size.y * 1.5f;
	}
}

std::array<float, 24> FontManager::GetCharacterVertices(const Character& ch_info,
                                                        const glm::vec2& position,
                                                        const float scale) const {
    const auto x_offset     = ch_info.bearing.x * scale;
    const auto y_offset     = (ch_info.size.y - ch_info.bearing.y) * scale;
    const glm::vec2 ch_pos  = position + glm::vec2{ x_offset, -y_offset };

    auto w = ch_info.size.x;
    auto h = ch_info.size.y;
    if (scale != 1.0f) {
        if (scale == 0.5f) {
            w >>= 1;
            h >>= 1;
        }
        else if (scale == 2.0f) {
            w <<= 1;
            h <<= 1;
        }
        else {
            w *= scale;
            h *= scale;
        }
    }

    return std::array<float, 24>{
            ch_pos.x,       ch_pos.y + h,   0.f, 0.f,
            ch_pos.x,       ch_pos.y,       0.f, 1.f,
            ch_pos.x + w,   ch_pos.y,       1.f, 1.f,
            ch_pos.x,       ch_pos.y + h,   0.f, 0.f,
            ch_pos.x + w,   ch_pos.y,       1.f, 1.f,
            ch_pos.x + w,   ch_pos.y + h,   1.f, 0.f
    };
}

unsigned FontManager::GetStringRenderWidth(const std::string& text,
                                           const float scale) const {
	unsigned width{};
	for (const auto& chr : text) {
	    const Character ch_info = (*m_font_in_use)[chr];
		width += static_cast<decltype(width)>((ch_info.advance >> 6) * scale);
	}

	return width;
}

void FontManager::Render(const Character& ch_info,
                         const std::array<float, 24>& vertices) const {
	// Render texture glyph
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ch_info.texture_id);

	// Update content of VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Render
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

} /*! opgs16::manager */
} /*! opgs16 */
