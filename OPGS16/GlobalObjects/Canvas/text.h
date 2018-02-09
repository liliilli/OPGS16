#ifndef OPGS16_GLOBAL_OBJECTS_CANVAS_TEXT_H
#define OPGS16_GLOBAL_OBJECTS_CANVAS_TEXT_H

/**
 * @file GlobalObjects\Canvas\text.h
 * @brief Text object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @date 2018-02-08
 */

#include <memory>       /** std::unique_ptr */
#include <glm\glm.hpp>  /** using glm::vec3<float, 0> = glm::tvec3<float, 0> */
#include "..\..\GlobalObjects\Interface\i_alignable.h"  /** IAlignable */
#include "..\..\Headers\Fwd\objectfwd.h"    /** FontManager
                                              * Canvas::TextImpl
                                              * Canvas::TextImplDeleter
                                              * glm::vec3 */
#include "..\..\System\Object\ui_object.h"  /** UiObject */

namespace Canvas {
/**
 * @class Text
 * @brief This class display text on position aligned with FontManager.
 */
class Text : public UiObject, public IAlignable {
public:
    /**
     * @brief Text component (stand-alone) constructor.
     * In initialization time of this class, initial text and position and color set.
     * position parameter was based on screen or parent's size where component is in hierarchy
     * structrue of parent.
     */
	Text(const std::string& initial_txt,
         const glm::vec3& position = glm::vec3{ 0.f, 0.f, 0.f },
         const glm::vec3& color = glm::vec3{ 1.f, 1.f, 1.f });

	virtual ~Text() = default;

    virtual void Update() override final {};

	virtual void Draw(helper::ShaderNew & shader) override final {};

	virtual void Draw() override final;

    /**
     * @brief Set text string to display.
     * @param[in] new_text New text string.
     */
	[[noreturn]] void SetText(const std::string& new_text);

    /**
     * @brief Get text string to display.
     * @return text string which this instance has.
     */
    const std::string GetText() const;

	/**
	 * @brief Set font size of string.
     * This method has side-effect for accessing FontManager.
	 * @param[in] size;
	 */
	[[noreturn]] void SetFontSize(const unsigned size);

    /**
     * @brief Get font size which this instance has.
     * @return unsigned font size value.
     */
    const unsigned GetFontSize() const;

	/**
	 * @brief Set this object font to render with.
	 * If FontManager doesn't find font, return false to inform method was failed.
	 * @param[in] font_tag Font name tag.
	 * @return Success flag.
	 */
	bool SetFontName(const std::string& font_tag);

    /**
     * @brief Set string color, single color with composited of {r, g, b}.
     * @param[in] color New color value with consists of {r, g, b} glm::vec3 vector.
     * each value must be in range of [0, 1], otherwise clamped to 0 or 1.
     */
    [[noreturn]] void SetColor(const glm::vec3& color);

    /**
     * @brief Get reference of FontManager which this class grasps.
     * Returning reference is attached to cv-qualifier (const), Caller can use const method
     * does not alter bit-consistancy of FontManager.
     * @return Const reference of FontManager which this class grasps (must not be a nullptr.)
     */
    inline const FontManager& GetFontManager() const {
        return *m_font_manager;
    }

private:
    /** private implementation instance */
    std::unique_ptr<TextImpl, TextImplDeleter> m_text_impl{ nullptr };
	FontManager* const m_font_manager{ nullptr };
};
}

#endif /** OPGS16_GLOBAL_OBJECTS_CANVAS_TEXT_H */
