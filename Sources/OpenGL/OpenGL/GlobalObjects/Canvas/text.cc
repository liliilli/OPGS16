#include "text.h"

Canvas::Text::Text(const std::string&& initial_text,
	const glm::vec3&& position,
	const glm::vec3&& color) :
	/** Body */
	m_text{ initial_text }, m_color{ color } {
	SetPosition(position);
}

void Canvas::Text::Update() {}

void Canvas::Text::Draw() {
	m_f_manager->RenderText(m_text, glm::vec2{ GetPosition() }, GetScaleValue(), m_color);
}

void Canvas::Text::SetText(const std::string && new_text) {
	m_text = new_text;
}
