#include "font_manager.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <glm\gtc\matrix_transform.hpp>
#include "..\application.h"

#include "..\..\System\Shader\shader_manager.h"

FontManager::FontManager() {
	{	/** Set Font rendering orthographic projection matrix. */
		auto& app = Application::getInstance();
		auto size = app.GetDefaultScreenSize();
		m_projection = glm::ortho(0.f, static_cast<float>(size[0]),
								0.f, static_cast<float>(size[1]));
	}

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

/**
 * @brief Initiate font glyph includes ASCII characters to use in program.
 *
 * @param[in] tag The tag refer to stored font glyphs later.
 * @param[in] font_path Font path to load.
 * @param[in] is_default The flag to set up this font to default in use.
 * default value is false (not default).
 *
 * @return The suceess flag.
 */
bool FontManager::InitiateFont(const std::string& tag, const std::string& font_path,
	bool is_default) {
	if (m_fonts.find(tag) != m_fonts.end()) {
		std::cerr << "ERROR::FREETYPE: There is already another font in tag." << std::endl;
		return false;
	}

	if (CheckFreeType(font_path)) {
		FT_Set_Pixel_Sizes(m_ft_face, 0, m_default_font_size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		// Create font and move it.
		m_fonts.insert(std::make_pair(tag, GetCharTextures()));
		// If caller ordere this font must be a default, insert raw pointer.
		if (is_default) m_default_font = m_fonts[tag].get();

		FT_Done_Face(m_ft_face);
		FT_Done_FreeType(m_freetype);
		return true;
	}
	else return false;
}

FontManager::font_map_ptr FontManager::GetCharTextures() {
	font_map_ptr glyphs = std::make_unique<font_map_ptr::element_type>();
	//std::unordered_map<GLchar, Character> glyphs{};

    for (GLubyte c = 0; c < 128; ++c) {
        if (FT_Load_Char(m_ft_face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate Texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        auto width  = m_ft_face->glyph->bitmap.width;
        auto height = m_ft_face->glyph->bitmap.rows;

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
        Character character {
            texture,
            glm::ivec2(width, height),
            glm::ivec2(m_ft_face->glyph->bitmap_left, m_ft_face->glyph->bitmap_top),
            static_cast<GLuint>(m_ft_face->glyph->advance.x)
        };

        glyphs->insert(std::make_pair(c, character));
    }

	return glyphs;
}

bool FontManager::LoadDefaultFont() {
	if (m_default_font == nullptr) return false;

	m_font_in_use = m_default_font;
	return true;
}

bool FontManager::LoadFont(const std::string& tag) {
	if (m_fonts.find(tag) == m_fonts.end()) return false;

	m_font_in_use = m_fonts.at(tag).get();
	return true;
}

bool FontManager::DeleteFont(const std::string& tag) {
	if (m_fonts.find(tag) == m_fonts.end()) return false;

	/** Remove pointer reference */
	auto font = m_fonts.at(tag).get();
	if (m_font_in_use == font) m_font_in_use = nullptr;
	if (m_default_font == font) m_default_font = nullptr;

	m_fonts.erase(tag);
	return true;
}

bool FontManager::CheckFreeType(const std::string& font_path) {
	// Check Freetype is well.
	if (FT_Init_FreeType(&m_freetype)) {
		std::cerr << "ERROR::FREETYPE: Could not init Freetype Library" << std::endl;
		return false;
	}

	if (FT_New_Face(m_freetype, font_path.c_str(), 0, &m_ft_face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}

	return true;
}

/**
 * @brief The method renders given text on given position with given color.
 *
 * This method is deprecated. (version 0.0.2~)
 *
 * @param[in] text String text
 * @param[in] pos Position text has to be shown on. x, y.
 * @param[in] scale Scale factor
 * @param[in] color Color to be colored.
 *
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 */
void FontManager::RenderText(std::string input, glm::vec2 input_pos, GLfloat scale, glm::vec3 color) {
	if (!m_font_in_use) return;

	m_shader->Use();
    m_shader->SetVec3f("textColor", color);
    m_shader->SetVecMatrix4f("projection", m_projection);

    glBindVertexArray(m_vao);

    /** Separate text to multi lines string */
    auto texts  = SeparateTextToList(input);
    auto pos    = input_pos;

    /** Render texts */
    for (const auto& text : texts) {
        // Iterate through all characters
        for (const auto& chr : text) {
            Character ch_info   = m_font_in_use->at(chr);

            auto x_offset = ch_info.bearing.x * scale;
            auto y_offset = (ch_info.size.y - ch_info.bearing.y) * scale;
            glm::vec2 ch_pos = pos + glm::vec2{ x_offset, -y_offset };

            auto w = ch_info.size.x * scale;
            auto h = ch_info.size.y * scale;

            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { ch_pos.x,     ch_pos.y + h,   0.0, 0.0 },
                { ch_pos.x,     ch_pos.y,       0.0, 1.0 },
                { ch_pos.x + w, ch_pos.y,       1.0, 1.0 },

                { ch_pos.x,     ch_pos.y + h,   0.0, 0.0 },
                { ch_pos.x + w, ch_pos.y,       1.0, 1.0 },
                { ch_pos.x + w, ch_pos.y + h,   1.0, 0.0 }
            };

            // Render texture glyph
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ch_info.texture_id);

            // Update content of VBO
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Render
            glDrawArrays(GL_TRIANGLES, 0, 6);
            pos.x += (ch_info.advance >> 6) * scale;
        }

        /** Relocate display position */
        pos.x   = input_pos.x;
        pos.y  -= m_font_in_use->at(0).size.y;
    }

	EndShader();
}

/**
 * @brief The method renders given text on given position with given color.
 *
 * This get text rendered with relative position from origin with color by aligning.
 * If text is multilined, text will be tokenized with '\n' return-carriage character.
 *
 * @param[in] text String text to be rendered.
 * @param[in] origin Origin position from which text strings rendered.
 * position bound is [0, screen_size], so DOWN_LEFT has position (0, 0) in Screen space.
 * In contrast UP_RIGHT will be (width, height) in Screen space.
 *
 * @param[in] relatve_position Relatve position from origin position string will be rendered.
 * Final position string rendered is (x, y) = (origin + relative_position + alignment_offset)
 *
 * @param[in] color The color to be rendered. R, G, B support.
 * @param[in] alignment String alignment parameter. default value is LEFT. (left-side)
 * @param[in] scale Scale factor value to apply it. Default value is 1.0f. (not-change)
 *
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 */
void FontManager::RenderTextNew
(const std::string&text, IOriginable::Origin origin, glm::vec2 relative_position, glm::vec3 color,
 IAlignable::Alignment alignment, const float scale) {
	if (m_font_in_use == nullptr) return;

	// Body
	StartShader(color);
	auto text_container = SeparateTextToList(text);
	auto position = relative_position;

	using Align = IAlignable::Alignment;
	switch (alignment) {
	case Align::LEFT:
		RenderLeftSide(text_container, position, scale);
		break;
	case Align::CENTER:
		RenderCenterSide(text_container, position, scale);
		break;
	case Align::RIGHT:
		RenderRightSide(text_container, position, scale);
		break;
	}

	EndShader();
}

/**
 * @brief This method starts shader with color to render.
 * @param[in] color The color to be attached to shader.
 */
void FontManager::StartShader(const glm::vec3& color) {
	m_shader->Use();
	m_shader->SetVec3f("textColor", color);
	m_shader->SetVecMatrix4f("projection", m_projection);
	glBindVertexArray(m_vao);
}

/**
 * @brief This method calculate and return barycenter position to render.
 *
 * @param[in] origin Origin position from which text strings rendered.
 * position bound is [0, screen_size], so DOWN_LEFT has position (0, 0) in Screen space.
 * In contrast UP_RIGHT will be (width, height) in Screen space.
 *
 * @param[in] position Relatve position from origin position string will be rendered.
 * Returned position string rendered is (x, y) = (origin + relative_position)
 *
 * @return The position has (x, y) value.
 */
glm::vec2 FontManager::CalculateCenterPosition(IOriginable::Origin& origin, glm::vec2& position) {
	/** x origin, y origin, width, height */
	std::array<GLint, 4> viewport{};
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);

	auto origin_type = static_cast<int>(origin) - 1;
	auto y_value = origin_type / 3;
	auto x_value = origin_type % 3;

	GLint x_pos = viewport[2] * x_value / 2;
	GLint y_pos = viewport[3] * y_value / 2;

	return glm::vec2(x_pos + position.x, y_pos + position.y);
}

/**
 * @brief Final render method actually renders strings from left side.
 *
 * @param[in] container Container stores multi-lined (separated) strings.
 * @param[in] position Position on which to render.
 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
 */
void FontManager::RenderLeftSide
(const std::vector<std::string>& container, const glm::vec2& position, const float scale) {
	/** Body */
	auto pos = position;

	for (const auto& t_text : container) {
		for (const auto& chr : t_text) {
			Character ch_info	= m_font_in_use->at(chr);
			auto vertices		= GetCharacterVertices(ch_info, pos, scale);

			Render(ch_info, vertices);
			pos.x += (ch_info.advance >> 6) * scale;
		}

		/** Relocate display position */
		pos.x = position.x;
		pos.y -= m_font_in_use->at(0).size.y * 1.5f;
	}
}

/**
 * @brief Final render method actually renders strings from center side.
 *
 * @param[in] container Container stores multi-lined (separated) strings.
 * @param[in] position Position on which to render.
 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
 */
void FontManager::RenderCenterSide
(const std::vector<std::string>& container, const glm::vec2& position, const float scale) {
	/** Body */
	auto pos = position;

	for (const auto& t_text : container) {
		pos.x -= GetStringRenderWidth(t_text, scale) / 2;

		for (const auto& chr : t_text) {
			Character ch_info = m_font_in_use->at(chr);
			auto vertices = GetCharacterVertices(ch_info, pos, scale);

			Render(ch_info, vertices);
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= m_font_in_use->at(0).size.y * 1.5f;
	}
}

/**
 * @brief Final render method actually renders strings from right side.
 *
 * @param[in] container Container stores multi-lined (separated) strings.
 * @param[in] position Position on which to render.
 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
 */
void FontManager::RenderRightSide
(const std::vector<std::string>& container, const glm::vec2& position, const float scale) {
	/** Body */
	auto pos = position;

	for (const auto& t_text : container) {
		pos.x -= GetStringRenderWidth(t_text, scale);

		for (const auto& chr : t_text) {
			Character ch_info = m_font_in_use->at(chr);
			auto vertices = GetCharacterVertices(ch_info, pos, scale);

			Render(ch_info, vertices);
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= m_font_in_use->at(0).size.y * 1.5f;
	}
}

/**
 * @brief The method gets character quad vertices to be needed for rendering.
 *
 * @param[in] info Specific character glyph information.
 * @param[in] position The position that character which will be rendered.
 * @param[in] scale Scale value to magnify or minify character render size.
 *
 * @return Character glyph render vertices information.
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 */
std::array<glm::vec4, 6> FontManager::GetCharacterVertices
(const Character& ch_info, const glm::vec2& pos, const float scale) {
	/** Body */
	auto x_offset = ch_info.bearing.x * scale;
	auto y_offset = (ch_info.size.y - ch_info.bearing.y) * scale;
	glm::vec2 ch_pos = pos + glm::vec2{ x_offset, -y_offset };

	auto w = ch_info.size.x * scale;
	auto h = ch_info.size.y * scale;

	return std::array<glm::vec4, 6>{
		glm::vec4{ ch_pos.x,	 ch_pos.y + h,	 0.0, 0.0 },
		glm::vec4{ ch_pos.x,     ch_pos.y,       0.0, 1.0 },
		glm::vec4{ ch_pos.x + w, ch_pos.y,       1.0, 1.0 },

		glm::vec4{ ch_pos.x,     ch_pos.y + h,   0.0, 0.0 },
		glm::vec4{ ch_pos.x + w, ch_pos.y,       1.0, 1.0 },
		glm::vec4{ ch_pos.x + w, ch_pos.y + h,   1.0, 0.0 }
	};
}

/**
 * @brief The method gets text and returns total rendering width size.
 *
 * @param[in] text One line string to measure.
 * @param[in] scale Scale value to magnify or minify character render size.
 *
 * @return The size
 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
 */
unsigned FontManager::GetStringRenderWidth(const std::string& text, const float scale) {
	unsigned width{};
	for (const auto& chr : text) {
		Character ch_info = m_font_in_use->at(chr);
		width += static_cast<decltype(width)>((ch_info.advance >> 6) * scale);
	}

	return width;
}

void FontManager::Render(const Character& ch_info, const std::array<glm::vec4, 6>& vertices) {
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

/**
 * @brief The method separate input to multi-lines strings detecting line-feed return carriage.
 * @param[in] text String text to separate
 * @return string list.
 */
std::vector<std::string> FontManager::SeparateTextToList(const std::string text) {
    std::vector<std::string> result;

    std::stringstream stream{text};
    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}

