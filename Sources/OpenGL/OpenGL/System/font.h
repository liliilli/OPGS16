#ifndef OPENGL_TUTORIAL_FONT_H
#define OPENGL_TUTORIAL_FONT_H

/**
 * @file System/font.h
 * @brief Fundamental font renderer to render string.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @version 0.0.2
 */

#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <ft2build.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include FT_FREETYPE_H

#include "Shader\shader.h"
#define PRIVATE__ private:

/**
 * @class Font
 * @brief The class manages reading fonts, rendering fonts.
 *
 * This class manages reading fonts, rendering fonts.
 * default shader is initiated as creating font instance.
 *
 * @todo Shader must be static instance to reduce memory usage.
 * @bug It seems that rendering does not work properly.
 */
class Font {
private:
    /**
     * @struct Chracter
     * @brief Container manages each texture of font glyphes.
     */
    struct Character {
        GLuint      textureID;

        /** glyph information */
        glm::ivec2  size;
        glm::ivec2  bearing;
        GLuint      advance;
    };

    helper::ShaderNew shader{};

public:
	/**
	 * @brief Initialize Font renderer instance with font path.
	 *
	 * @param[in] font_path Font path to be rendered. this parameter is only for r-value.
	 */
    Font(std::string&& font_path);

	enum class FontOrigin : int {
		DOWN_LEFT = 1,		DOWN_CENTER = 2,	DOWN_RIGHT = 3,
		CENTER_LEFT = 4,	CENTER_CENTER = 5,	CENTER_RIGHT = 6,
		UP_LEFT = 7,		UP_CENTER = 8,		UP_RIGHT = 9
	};

	enum class FontAlignment { LEFT, CENTER, RIGHT };

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
    [[noreturn]]
	[[deprecated("Use RenderTextNew instead. this method does not support alignment features.")]]
	void RenderText(std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color);

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
	[[noreturn]] void RenderTextNew(const std::string& text, FontOrigin origin,
		glm::vec2 relative_position, glm::vec3 color,
		FontAlignment alignment = FontAlignment::LEFT, const float scale = 1.0f);

private:
	/**
	 * @brief This method starts shader with color to render.
	 * @param[in] color The color to be attached to shader.
	 */
	[[noreturn]] void StartShader(const glm::vec3& color);

    /**
     * @brief The method sets character textures from glyphs and store them to container.
     *
     * This methods called when initiate instance.
     */
    [[noreturn]] void GetCharTextures();

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
	glm::vec2 CalculateCenterPosition(FontOrigin& origin, glm::vec2& position);

	/**
	 * @brief This methods ends shader's operation.
	 */
	[[noreturn]] inline void EndShader() {
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/**
	 * @brief Final render method actually renders strings from left side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	[[noreturn]] void RenderLeftSide(const std::vector<std::string>& container,
		const glm::vec2& position, const float scale);

	/**
	 * @brief Final render method actually renders strings from center side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	[[noreturn]] void RenderCenterSide(const std::vector<std::string>& container,
		const glm::vec2& position, const float scale);

	/**
	 * @brief Final render method actually renders strings from right side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	[[noreturn]] void RenderRightSide(const std::vector<std::string>& container,
		const glm::vec2& position, const float scale);

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
	std::array<glm::vec4, 6> GetCharacterVertices
	(const Character& info, const glm::vec2& position, const float scale);

	/**
	 * @brief The method gets text and returns total rendering width size.
	 *
	 * @param[in] text One line string to measure.
	 * @param[in] scale Scale value to magnify or minify character render size.
	 *
	 * @return The size
	 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
	 */
	unsigned GetStringRenderWidth(const std::string& text, const float scale);

	/**
	 * @brief Actual render method. This method must be called in Render__Side() method.
	 *
	 * @param[in] ch_info
	 * @param[in] vertices
	 */
	[[noreturn]] void Render(const Character& ch_info, const std::array<glm::vec4, 6>& vertices);

    /**
     * @brief The method sets VAO, VBO to render string on screen.
     *
     * This methods called when initiate instance.
     */
    [[noreturn]] void BindVertexAttributes();

    /**
     * @brief The method separate input to multi-lines strings detecting line-feed return carriage.
     * @param[in] text String text to separate
     * @return string list.
     */
    std::vector<std::string> SeparateTextToList(const std::string text);

    /** Freetype pointer */
    FT_Library freetype;
    FT_Face face;

    // Restrict first 128 characters for now.
    std::unordered_map<GLchar, Character> characters;
    std::array<GLuint, 4> viewport_size;
    glm::mat4 projection;

    GLuint vao, vbo;
};

#endif // OPENGL_TUTORIAL_FONT_H
