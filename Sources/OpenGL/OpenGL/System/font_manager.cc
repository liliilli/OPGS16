#include "font_manager.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <glm\gtc\matrix_transform.hpp>

#include "..\..\System\Shader\shader_manager.h"

FontManager::FontManager() {
	InitiateShader();
	BindVertexAttributes();
}

void FontManager::GetCharTextures() {
    for (GLubyte c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate Texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        auto width  = face->glyph->bitmap.width;
        auto height = face->glyph->bitmap.rows;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     width, height,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        // Set Texture Options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character {
            texture,
            glm::ivec2(width, height),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };

        characters.insert(std::make_pair(c, character));
    }
}

void FontManager::BindVertexAttributes() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool FontManager::InitiateFont(const std::string&& font_path) {
	// Check Freetype is well.
	if (FT_Init_FreeType(&freetype)) {
		std::cerr << "ERROR::FREETYPE: Could not init Freetype Library" << std::endl;
		return false;
	}

	if (FT_New_Face(freetype, font_path.c_str(), 0, &face)) {
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GetCharTextures();
	FT_Done_Face(face);
	FT_Done_FreeType(freetype);

	return true;
}

/**
 * @brief The method renders given text on given position with given color.
 *
 * @param[in] text String text
 * @param[in] pos Position text has to be shown on. x, y.
 * @param[in] scale Scale factor
 * @param[in] color Color to be colored.
 */
void FontManager::RenderText(std::string input, glm::vec2 input_pos, GLfloat scale, glm::vec3 color) {
    shader->Use();
    shader->SetVec3f("textColor", color);
    shader->SetVecMatrix4f("projection", projection);

    glBindVertexArray(vao);

    /** Separate text to multi lines string */
    auto texts  = SeparateTextToList(input);
    auto pos    = input_pos;

    /** Render texts */
    for (const auto& text : texts) {
        // Iterate through all characters
        for (const auto& chr : text) {
            Character ch_info   = characters.at(chr);

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
            glBindTexture(GL_TEXTURE_2D, ch_info.textureID);

            // Update content of VBO
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Render
            glDrawArrays(GL_TRIANGLES, 0, 6);
            pos.x += (ch_info.advance >> 6) * scale;
        }

        /** Relocate display position */
        pos.x   = input_pos.x;
        pos.y  -= characters.at(0).size.y;
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
(const std::string& text, FontOrigin origin, glm::vec2 relative_position, glm::vec3 color,
	FontAlignment alignment, const float scale) {
	// Body
	StartShader(color);
	auto text_container = SeparateTextToList(text);
	auto position = CalculateCenterPosition(origin, relative_position);

	switch (alignment) {
	case FontAlignment::LEFT:
		RenderLeftSide(text_container, position, scale);
		break;
	case FontAlignment::CENTER:
		RenderCenterSide(text_container, position, scale);
		break;
	case FontAlignment::RIGHT:
		RenderRightSide(text_container, position, scale);
		break;
	}

	EndShader();
}

void FontManager::InitiateShader() {
	auto& manager = ShaderManager::GetInstance();
	shader = manager.GetShaderWithName("gCommonFont");
	if (!shader) {
		using Type = helper::ShaderNew::Type;
		using namespace std::string_literals;

		shader = manager.CreateShader("gCommonFont", {
			{Type::VS, "Shaders/Global/font.vert"s},
			{Type::FS, "Shaders/Global/font.frag"s}
			});
	}
}

/**
 * @brief This method starts shader with color to render.
 * @param[in] color The color to be attached to shader.
 */
void FontManager::StartShader(const glm::vec3& color) {
	shader->Use();
	shader->SetVec3f("textColor", color);
	shader->SetVecMatrix4f("projection", projection);
	glBindVertexArray(vao);
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
glm::vec2 FontManager::CalculateCenterPosition(FontOrigin& origin, glm::vec2& position) {
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
			Character ch_info	= characters.at(chr);
			auto vertices		= GetCharacterVertices(ch_info, pos, scale);

			Render(ch_info, vertices);
			pos.x += (ch_info.advance >> 6) * scale;
		}

		/** Relocate display position */
		pos.x = position.x;
		pos.y -= characters.at(0).size.y;
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
			Character ch_info = characters.at(chr);
			auto vertices = GetCharacterVertices(ch_info, pos, scale);

			Render(ch_info, vertices);
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= characters.at(0).size.y;
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
			Character ch_info = characters.at(chr);
			auto vertices = GetCharacterVertices(ch_info, pos, scale);

			Render(ch_info, vertices);
			pos.x += (ch_info.advance >> 6) * scale;
		}

		pos.x = position.x;
		pos.y -= characters.at(0).size.y;
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
		Character ch_info = characters.at(chr);
		width += (ch_info.advance >> 6) * scale;
	}

	return width;
}

void FontManager::Render(const Character& ch_info, const std::array<glm::vec4, 6>& vertices) {
	// Render texture glyph
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ch_info.textureID);

	// Update content of VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

