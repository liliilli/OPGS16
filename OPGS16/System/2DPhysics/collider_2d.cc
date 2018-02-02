#include "collider_2d.h"

namespace collision {

void Collider2D::SetCollisionType(CollisionType type) {
	m_collision_type = type;
}

Collider2D::CollisionType Collider2D::GetCollisionType() {
	return m_collision_type;
}

}
