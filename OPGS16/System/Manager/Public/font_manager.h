#ifndef OPGS16_SYSTEM_FONT_MANAGER_H
#define OPGS16_SYSTEM_FONT_MANAGER_H

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

/**
 * @file System/Manager/Public/font_manager.h
 * @brief Fundamental font renderer to render string, and manages font glyphs.
 *
 * This file consists of application operation class and member API functions.
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-06 Create file.
 * 2018-03-03 Refactoring.
 */

#include <array>
#include <string>
#include <iostream>
#include <memory>
#include <map>

#include <ft2build.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H

#include "../Internal/font_internal.h"
#include "../../Shader/shader.h"
#include "../../../GlobalObjects/Interface/i_originable.h"
#include "../../../GlobalObjects/Interface/i_alignable.h"

namespace opgs16 {
namespace manager {

/*! Default font size variable */
constexpr unsigned k_default_font_size = 16u;

/*!
 * @class FontManager
 * @brief The class manages reading fonts, rendering fonts.
 *
 * This class manages reading fonts, rendering fonts.
 * default shader is initiated as creating font instance.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-03 Refactoring. Move it to opgs16::manager.
 *
 * @todo Shader must be static instance to reduce memory usage.
 */
class FontManager final {
public:

	/** Internal type aliasing */
	using font_type		= std::map<GLchar, _internal::Character>;
	using font_map_ptr	= std::unique_ptr<font_type>;
	using font_raw		= font_type*;

public:
	/*! Return single static instance. This must be called in initiation time once. */
	static FontManager& Instance() {
		static FontManager instance{};
		return instance;
	}

    /*!
     * @brief Generate font glyphs with name_tag from ResourceManager.
     * @param[in] name_tag Name tag to refer to stored font glyphs later,
     * and get information from ResourceManager.
     *
     * @return The success flag.
     */
    bool GenerateFont(const std::string& name_tag);

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
	 * @param[in] font_tag Font name to find in font container.
	 * @return The success flag. If font is exist, return true.
	 */
	inline bool DoesFontExist(const std::string& font_tag) const {
        return m_fonts.find(font_tag) != m_fonts.end();
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
	void RenderTextNew(const std::string& text, IOriginable::Origin origin,
                       const glm::vec2 final_position, const glm::vec3 color,
                       IAlignable::Alignment alignment = IAlignable::Alignment::LEFT,
                       const float scale = 1.0f);

private:
    /** Freetype pointer */
	FT_Library	m_freetype{ nullptr };	/** Freetype library pointer */
	FT_Face		m_ft_face{ nullptr };	/** Freetype face pointer used when initiating fonts. */

    // Restrict first 128 characters for now.
	std::unordered_map<std::string, font_map_ptr> m_fonts{};	/** Container stores fonts */
	font_raw	m_font_in_use{ nullptr };
	font_raw	m_default_font{ nullptr };

    const glm::mat4 m_projection;
    GLuint m_vao{}, m_vbo{};

	helper::ShaderNew* m_shader{};

private:
	/**
	 * @brief This method starts shader with color to render.
	 * @param[in] color The color to be attached to shader.
	 */
	void StartShader(const glm::vec3& color) const;

	/**
	 * @brief Checks freetype pointer with FontPath.
	 * @param[in] font_path Font's path to refer.
	 * @return If checking is successful, return true. otherwise return false.
	 */
    inline bool CheckFreeType() noexcept {
        // Check Freetype is well.
        if (FT_Init_FreeType(&m_freetype)) {
            std::cerr << "ERROR::FREETYPE: Could not init Freetype Library\n";
            return false;
        }
        else return true;
    }

    inline bool LoadFreeType(const std::string& font_path) noexcept {
        if (FT_New_Face(m_freetype, font_path.c_str(), 0, &m_ft_face)) {
            std::cerr << "ERROR::FREETYPE: Failed to load font\n";
            return false;
        }
        else return true;
    }

    /**
     * @brief The method sets character textures from glyphs and store them to container.
     * This methods called when initiate instance.
	 * @return Created font glyph container unique_ptr (moved)
     */
    FontManager::font_map_ptr GetCharTextures() const;

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
                        const glm::vec2& position, const float scale) const;

	/**
	 * @brief Final render method actually renders strings from center side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	void RenderCenterSide(const std::vector<std::string>& container,
                          const glm::vec2& position, const float scale) const;

	/**
	 * @brief Final render method actually renders strings from right side.
	 *
	 * @param[in] container Container stores multi-lined (separated) strings.
	 * @param[in] position Position on which to render.
	 * @param[in] scale Scale factor, it magnify or minify rendered string textures.
	 */
	void RenderRightSide(const std::vector<std::string>& container,
                         const glm::vec2& position, const float scale) const;

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
	std::array<float, 24> GetCharacterVertices
	(const _internal::Character& info, const glm::vec2& position, const float scale) const;

	/**
	 * @brief The method gets text and returns total rendering width size.
	 *
	 * @param[in] text One line string to measure.
	 * @param[in] scale Scale value to magnify or minify character render size.
	 *
	 * @return The size
	 * @see https://www.freetype.org/freetype2/docs/tutorial/step2.html
	 */
	unsigned GetStringRenderWidth(const std::string& text, const float scale) const;

	/**
	 * @brief Actual render method. This method must be called in Render__Side() method.
	 *
	 * @param[in] ch_info
	 * @param[in] vertices
	 */
	void Render(const _internal::Character& ch_info, const std::array<float, 24>& vertices) const;

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
    std::vector<std::string> SeparateTextToList(const std::string& text) const;

private:
	/**
	 * @brief Initialize Font renderer instance with font path.
	 * @param[in] font_path Font path to be rendered. this parameter is only for r-value.
	 */
	FontManager();

public:
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
};

} /*! opgs16::manager */
} /*! opgs16 */

#endif /*! OPGS16_SYSTEM_FONT_MANAGER_H */
