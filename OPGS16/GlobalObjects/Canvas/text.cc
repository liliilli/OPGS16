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
	if (m_f_manager) {
		/** Set font */
		if (m_font_tag.empty()) m_f_manager->LoadDefaultFont();
		else m_f_manager->LoadFont(std::string(m_font_tag));
		/** Render */
		m_f_manager->RenderTextNew(m_text, GetOrigin(),
			glm::vec2{ GetFinalPosition() }, m_color, GetAlignment(), GetScaleValue());
	}
	else { std::cerr << "ERROR::FONT_MANAGER CAN NOT FIND::CRITICAL" << std::endl; }
}

void Canvas::Text::SetText(const std::string&& new_text) {
	m_text = new_text;
}

void Canvas::Text::SetFontSize(const unsigned size) {
	auto def = m_f_manager->GetDefaultFontSize();
	SetScaleValue(static_cast<float>(size) / static_cast<float>(def));
}

bool Canvas::Text::SetFont(const std::string&& font_tag) {
	if (m_f_manager && m_f_manager->IsFontExist(font_tag)) {
		m_font_tag = std::move(font_tag);
		return true;
	}
	else {
		std::cerr << "ERROR::FONT::NOT::FOUND" << std::move(font_tag) << std::endl;
		m_font_tag = "";
		return false;
	}
}
