#include "rectangle.h"

namespace collision {

RectangleCollider2D::RectangleCollider2D(const float width, const float height) :
	m_width{ width }, m_height{ height } {
	RectangleCollider2D(-m_width / 2.f, m_height / 2.f, m_width / 2.f, -m_height / 2.f);
}

RectangleCollider2D::RectangleCollider2D(const float left,
	const float up, const float right, const float down) {
	/** Body */
	SetRegion(left, up, right, down);
}

void RectangleCollider2D::SetRegion(const float left,
	const float up, const float right, const float down) {
	/** Body */
	m_left = left;	if (m_left < 0) m_left = 0;
	m_up = up;		if (m_up < 0) m_up = 0;
	m_right = right;if (m_right < 0) m_right = 0;
	m_down = down;	if (m_down < 0) m_down = 0;
	/** Fix width, height again */
	m_width = m_right - m_left;
	m_height = m_up - m_down;
	/** Set lu ld ru rd */
	UpdateAllCorner(m_left, m_up, m_right, m_down);
}

void RectangleCollider2D::UpdateAllCorner(const float m_left,
	const float m_up, const float m_right, const float m_down) {
	/** Body */
	lu = glm::vec2{ m_left, m_up };
	ld = glm::vec2{ m_left, m_down };
	ru = glm::vec2{ m_right, m_up };
	rd = glm::vec2{ m_right, m_down };
}

}
