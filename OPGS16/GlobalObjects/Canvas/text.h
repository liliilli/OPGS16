#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_TEXT_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_TEXT_H

/**
 * @file GlobalObjects\Canvas\text.h
 * @brief Text object used in Canvas hierarchy.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include "..\Interface\i_originable.h"
#include "..\..\Headers\common.h"
#include "..\..\System\font_manager.h"

namespace Canvas {
/**
 * @class Text
 * @brief This class display text on position aligned with FontManager.
 */
class Text : public UiObject, public IAlignable {
public:
	Text(const std::string&& initial_text,
		const glm::vec3&& position = { 0, 0, 0 },
		const glm::vec3&& color = { 1, 1, 1 });
	Text(const std::string& initial_txt,
		const glm::vec3& position = { 0, 0, 0 },
		const glm::vec3& color = { 1, 1, 1 });
	Text() = delete;

	virtual ~Text() = default;

	virtual void Update() override final;

	virtual void Draw(helper::ShaderNew & shader) override final {};

	virtual void Draw() override final;

	[[noreturn]] void SetText(const std::string& new_text);

	/**
	 * @brief Set font size of string.
	 * @param[in] size;
	 */
	[[noreturn]] void SetFontSize(const unsigned size);

	/**
	 * @brief Set this object font to render with.
	 * If FontManager doesn't find font, return false to inform method was failed.
	 *
	 * @param[in] font_tag Font name tag.
	 * @return Success flag.
	 */
	bool SetFont(const std::string& font_tag);

private:
	std::string m_text{};
	FontManager* m_f_manager = &FontManager::GetInstance();

	using fontMap = FontManager::font_map_ptr;
	std::string m_font_tag{};

	glm::vec3 m_color{};
};
}

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_CANVAS_TEXT_H */
