#ifndef OPGS16_SYSTEM_2D_PHYSICS_COLLIDER_2D_H
#define OPGS16_SYSTEM_2D_PHYSICS_COLLIDER_2D_H
#include <string>

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Components/Physics2D/collider_2d.h
 * @brief Base collider class of all 2d colliders.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-17 Add boilerplate codes, Add collision layer index,
 * Move it to ::opgs16::element::physics
 */

namespace opgs16 {
namespace element {
namespace physics {

} /*! opgs16::element::physics */
} /*! opgs16::element */
} /*! opgs16 */

/*!
 * @namespace collision
 * @brief
 */
namespace collision {

/**
 * @class CCollider2D
 * @brief
 */
class CCollider2D {
public:
	/**
	 * @brief
	 */
	enum class ECollisionType {
		TRIGGER,	/*! TRIGGER type is not affected by action and reaction. */
		COLLISION,	/*! COLLISION type is affected by action and reaction. */
	};

	/**
	 * @brief Set collision type, either TRIGGER or COLLISION.
	 * @param[in] type Collision type.
	 */
	void SetCollisionType(ECollisionType type);

	/*! Return collision type, either TRIGGER or COLLISION. */
	inline ECollisionType CollisionType() const noexcept {
        return m_collision_type;
	}

    /*! Set collision layer value. If it's out of bound, assert runtime_error. */
    void SetCollisionLayerIndex(unsigned layer_value);

    /*! Set collision layer value with string name, It it's out of bound, assert error. */
    void SetCollisionLayerName(const std::string& layer_name);

    /*! Return collision layer value, unsigned int. */
    inline unsigned CollisionLayer() const noexcept {
        return m_collision_layer_index;
    }

    /*! Return collision layer value with string. */
    std::string CollisionLayerStringOf() const;

private:
	ECollisionType m_collision_type{ ECollisionType::COLLISION };   /*! Collision type. */

    unsigned m_collision_layer_index{ 0 };      /*! Collision layer index. */
};

}

#endif /** OPGS16_SYSTEM_2D_PHYSICS_COLLIDER_2D_H */