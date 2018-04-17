#ifndef SYSTEM_2D_PHYSICS_COLLIDER_RECTANGLE_H
#define SYSTEM_2D_PHYSICS_COLLIDER_RECTANGLE_H

/**
 * @file System\2DPhysics\Collider\rectangle.h
 * @brief Rectangle collider.
 *
 * This file consists of rectangle collider member API functions, data.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <glm\glm.hpp>
#include "..\collider_2d.h"

/**
 * @namespace collision
 * @brief
 */
namespace collision {

/**
 * @class CRectangleCollider2D
 * @brief
 */
class CRectangleCollider2D : public CCollider2D {
public:
	/**
	 * @brief Constructor with left, up, right, down. (default)
	 */
	CRectangleCollider2D(const float left, const float up, const float right, const float down);

	/**
	 * @brief Set region using with left, up, right, down boundary axis dependent value.
	 * @param[in] left Left border position value from source position.
	 * @param[in] up Up border position value from source position.
	 * @param[in] right Right border position value from source position.
	 * @param[in] down Down border position value from source position.
	 */
	 void SetRegion(const float left, const float up,
                                const float right, const float down);

    /*!
     * @brief
     */
     void ReflectPosition(const glm::vec3& position);

    enum class PositionType {
        LEFT_DOWN,  /*! Left down position */
        RIGHT_UP,   /*! Right up position */
    };

    /*!
     * @brief
     * @return
     */
    glm::vec2 GetTipPosition(PositionType type) const;

private:
	/**
	 * This four values are based on local coordinates barycentric of object to bind.
	 * \(lu)___/(ru) lu is left-up position from center point.
	 * |   .(c)|     ru is right-up position from center point.
	 * /(ld)---\(rd) ld is left-down position from center point.
	 * rd is right-down position from center point.
	 *
	 * This values are able to be manipulated by calling methods.
	 */
	glm::vec2 lu{}, ru{}, ld{}, rd{};

    mutable glm::vec2 final_lu{}, final_ru{}, final_ld{}, final_rd{};
    glm::vec3 m_position_of_bound_object{};

	/** This values are automatically refreshed by calling method which sets lu ru ld rd.*/
	float m_left{}, m_up{}, m_right{}, m_down{};
	float m_width{}, m_height{};

    mutable bool m_position_changed{ true };
private:
	/**
	 * @brief Update all corners coordinate values internally.
	 * @param[in] left Left border position value from source position.
	 * @param[in] up Up border position value from source position.
	 * @param[in] right Right border position value from source position.
	 * @param[in] down Down border position value from source position.
	 */
	 void UpdateAllCorner(const float left, const float up,
                                      const float right, const float down);

     void UpdateFinalCorner() const;
};

}

#endif /** SYSTEM_2D_PHYSICS_COLLIDER_RECTANGLE_H */
