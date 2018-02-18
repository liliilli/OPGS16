#ifndef OPENGL_TUTORIAL_SYSTEM_FONT_MANAGER_H
#define OPENGL_TUTORIAL_SYSTEM_FONT_MANAGER_H

/**
 * @file System/font_manager.h
 * @brief Fundamental font renderer to render string, and manages font glyphs.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @date 2018 - 02 - 06
 */

#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <ft2build.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include FT_FREETYPE_H

#include "..\Shader\shader.h"
#include "..\..\GlobalObjects\Interface\i_originable.h"
#include "..\..\GlobalObjects\Interface\i_alignable.h"

/**
 * @class FontManager
 * @brief The class manages reading fonts, rendering fonts.
 *
 * This class manages reading fonts, rendering fonts.
 * default shader is initiated as creating font instance.
 *
 * @todo Shader must be static instance to reduce memory usage.
 */
class FontManager final {
public:
    /**
     * @struct Chracter
     * @brief Container manages each texture of font glyphes.
     */
    struct Character {
        GLuint      texture_id;	/** texture id of each character. */

        /** glyph information */
        glm::ivec2  size;		/** glyph size */
        glm::ivec2  bearing;	/** glyph bearing position */
        GLuint      advance;	/** glyph advance width */
    };

	/** Internal type aliasing */
	using font_type		= std::unordered_map<GLchar, Character>;
	using font_raw		= font_type*;
	using font_map_ptr	= std::unique_ptr<font_type>;

public:
	/**
	 * @brief Return single static instance. This must be called in initiation time once.
	 * @return The reference of FontManager instance.
	 */
	static FontManager& GetInstance() {
		static FontManager instance{};
		return instance;
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
	bool InitiateFont(const std::string& tag, const std::string& font_path,
		bool is_default = false);

	/**
	 * @brief Loads default font. If default is not specified, return false flag.
	 * @return The success flag.
	 */
	bool LoadDefaultFont();

	/**
	 * @brief Loads font that is specified by tag name. if not found, return false flag.
	 * @param[in] tag The tag refers to key load font glyphs.
	 * @return The success flag.
	 */
	bool LoadFont(const std::string& tag);

	/**
	 * @brief Delete font that is specified by tag name. if not found, return false flag.
	 * @param[in] tag The tag destroy stored font glyphs.
	 * @reutrn The success flag.
	 */
	bool DeleteFont(const std::string& tag);

	/**
	 * @brief Check font exists.
	 * @param[in] font_name Font name to find in font container.
	 * @return The success flag. If font is exist, return true.
	 */
	inline bool DoesFontExist(const std::string& font_name);

    /**
     * @brief Get defult font size value, set at initialization time of FontManager.
     * @return Default font size.
     */
    inline unsigned GetDefaultFontSize() const {
        return m_default_font_size;
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
	 void RenderTextNew(const std::string& text, IOriginable::Origin origin,
		glm::vec2 relative_position, glm::vec3 color,
		IAlignable::Alignment alignment = IAlignable::Alignment::LEFT, const float scale = 1.0f);

private:
    /** Freetype pointer */
	FT_Library	m_freetype{ nullptr };	/** Freetype library pointer */
	FT_Face		m_ft_face{ nullptr };	/** Freetype face pointer used when initiating fonts. */

    // Restrict first 128 characters for now.
	std::unordered_map<std::string, font_map_ptr> m_fonts{};	/** Container stores fonts */
	font_raw	m_font_in_use{ nullptr };
	font_raw	m_default_font{ nullptr };

    std::array<GLuint, 4> m_viewport_size;
	glm::mat4 m_projection;

    GLuint m_vao, m_vbo;

	helper::ShaderNew* m_shader{};
	const unsigned m_default_font_size = 16u;

private:
	/**
	 * @brief This method starts shader with color to render.
	 * @param[in] color The color to be attached to shader.
	 */
	 void StartShader(const glm::vec3& color);

	/**
	 * @brief Checks freetype pointer with FontPath.
	 * @param[in] font_path Font's path to refer.
	 * @return If checking is successful, return true. otherwise return false.
	 */
	bool CheckFreeType(const std::string& font_path);

    /**
     * @brief The method sets character textures from glyphs and store them to container.
     * This methods called when initiate instance.
	 * @return Created font glyph container unique_ptr (moved)
     */
    FontManager::font_map_ptr GetCharTextures();

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
	glm::vec2 CalculateCenterPosition(IOriginable::Origin& origin, glm::vec2& position);

	/**
	 * @brief This methods ends shader's operation.
	 */
	 inline void EndShader() {
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
	 void RenderLeftSide(const std::vector<std::string>& container,
		const glm::vec2& position, const float scale);

	/**
	 * @brief Final render method actually renders strings from center side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	 void RenderCenterSide(const std::vector<std::string>& container,
		const glm::vec2& position, const float scale);

	/**
	 * @brief Final render method actually renders strings from right side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	 void RenderRightSide(const std::vector<std::string>& container,
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
	 void Render(const Character& ch_info, const std::array<glm::vec4, 6>& vertices);

    /**
     * @brief The method sets VAO, VBO to render string on screen.
     *
     * This methods called when initiate instance.
     */
     void BindVertexAttributes();

    /**
     * @brief The method separate input to multi-lines strings detecting line-feed return carriage.
     * @param[in] text String text to separate
     * @return string list.
     */
    std::vector<std::string> SeparateTextToList(const std::string text);

private:
	/**
	 * @brief Initialize Font renderer instance with font path.
	 * @param[in] font_path Font path to be rendered. this parameter is only for r-value.
	 */
	FontManager();
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager(const FontManager&&) = delete;
	FontManager&& operator=(const FontManager&&) = delete;
};

inline bool FontManager::DoesFontExist(const std::string& tag) {
	if (m_fonts.find(tag) == m_fonts.end()) return false;
	else return true;
}

#endif // OPENGL_TUTORIAL_SYSTEM_FONT_MANAGER_H
