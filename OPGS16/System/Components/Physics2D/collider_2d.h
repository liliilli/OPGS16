#ifndef OPGS16_SYSTEM_2D_PHYSICS_COLLIDER_2D_H
#define OPGS16_SYSTEM_2D_PHYSICS_COLLIDER_2D_H

/**
 * @namespace collision
 * @brief
 */
namespace collision {

/**
 * @class Collider2D
 * @brief
 */
class Collider2D {
public:
	/**
	 * @brief
	 */
	enum class CollisionType {
		TRIGGER,	/** TRIGGER type is not affected by action and reaction. */
		COLLISION,	/** COLLISION type is affected by action and reaction. */
	};

	/**
	 * @brief Set collision type, either TRIGGER or COLLISION.
	 * @param[in] type Collision type.
	 */
	[[noreturn]] void SetCollisionType(CollisionType type);

	/**
	 * @brief Get collision type, either TRIGGER or COLLISION.
	 * @return Collision type value.
	 */
	CollisionType GetCollisionType();

private:
	CollisionType m_collision_type{ CollisionType::COLLISION };
};

}

#endif /** OPGS16_SYSTEM_2D_PHYSICS_COLLIDER_2D_H */