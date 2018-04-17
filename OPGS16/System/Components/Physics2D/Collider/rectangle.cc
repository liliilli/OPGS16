#include "rectangle.h"          /*! Header file */

namespace collision {

CRectangleCollider2D::CRectangleCollider2D(const float left, const float up,
                                         const float right, const float down) {
	SetRegion(left, up, right, down);
}

void CRectangleCollider2D::SetRegion(const float left, const float up,
                                    const float right, const float down) {
	/** Body */
	m_left = left;
	m_up = up;
	m_right = right;
	m_down = down;

	/** Fix width, height again */
	m_width = m_right - m_left;
	m_height = m_up - m_down;

    /** Set lu ld ru rd */
	UpdateAllCorner(m_left, m_up, m_right, m_down);
}

void CRectangleCollider2D::ReflectPosition(const glm::vec3& position) {
    m_position_of_bound_object = position;

    m_position_changed = true;
    UpdateFinalCorner();
}

glm::vec2 CRectangleCollider2D::GetTipPosition(PositionType type) const {
    if (m_position_changed)
        UpdateFinalCorner();

    switch (type) {
    case PositionType::LEFT_DOWN:
        return final_ld;
    case PositionType::RIGHT_UP:
        return final_ru;
    }
}

void CRectangleCollider2D::UpdateAllCorner(const float m_left,
	const float m_up, const float m_right, const float m_down) {
	/** Body */
	lu = glm::vec2{ m_left, m_up };
	ld = glm::vec2{ m_left, m_down };
	ru = glm::vec2{ m_right, m_up };
	rd = glm::vec2{ m_right, m_down };

    m_position_changed = true;
    UpdateFinalCorner();
}

void CRectangleCollider2D:: UpdateFinalCorner() const {
    if (m_position_changed) {
        glm::vec2 pos = m_position_of_bound_object;
        final_lu = lu + pos;
        final_ld = ld + pos;
        final_ru = ru + pos;
        final_rd = rd + pos;

        m_position_changed = false;
    }
}

}
