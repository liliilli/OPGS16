#include "text.h"

Canvas::Text::Text(const std::string&& initial_text,
	const glm::vec3&& position,
	const glm::vec3&& color) :
	/** Body */
	m_text{ initial_text }, m_color{ color } {
	SetLocalPosition(position);
}

Canvas::Text::Text(const std::string & initial_txt,
	const glm::vec3 & position,
	const glm::vec3 & color) :
	/** Body */
	m_text{ initial_txt }, m_color{ color } {
	SetLocalPosition(position);
}

void Canvas::Text::Update() {}

void Canvas::Text::Draw() {
	m_f_manager->RenderTextNew(m_text, GetOrigin(),
		glm::vec2{ GetFinalPosition() }, m_color, GetAlignment(), GetScaleValue());
}

void Canvas::Text::SetText(const std::string && new_text) {
	m_text = new_text;
}
