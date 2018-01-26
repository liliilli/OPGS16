#ifndef OPENGL_TUTORIALS_GLOBAL_OBJECTS_INTERFACE_I_ORIGINABLE_H
#define OPENGL_TUTORIALS_GLOBAL_OBJECTS_INTERFACE_I_ORIGINABLE_H

#include <glm/glm.hpp>

class IOriginable {
public:
	/**
	 * @brief
	 */
	enum class Origin : int {
		UP_LEFT = 7,		UP_CENTER = 8,		UP_RIGHT = 9,
		CENTER_LEFT = 4,	CENTER_CENTER = 5,	CENTER_RIGHT = 6,
		DOWN_LEFT = 1,		DOWN_CENTER = 2,	DOWN_RIGHT = 3
	};

	/**
	 * @brief
	 * @param[in] value
	 */
	[[noreturn]] inline void SetOrigin(Origin value);

	/**
	 * @brief
	 * @return Origin type value.
	 */
	inline Origin GetOrigin() const;

	/**
	 * @brief This method calculate and return barycenter position to render.
	 *
	 * @param[in] origin Origin position from which text strings rendered.
	 * position bound is [0, screen_size], so DOWN_LEFT has position (0, 0) in Screen space.
	 * In contrast UP_RIGHT will be (width, height) in Screen space.
	 *
	 * @param[in] position Relatve position from origin position string will be rendered.
	 * Returned position string rendered is (x, y) = (origin + relative_position)
	 *
	 * @return The position has (x, y) value.
	 */
	glm::vec2 CalculateCenterPosition(const Origin& origin, const glm::vec2& position) const;

private:
	Origin origin{ Origin::CENTER_CENTER };
};

#endif /** OPENGL_TUTORIALS_GLOBAL_OBJECTS_INTERFACE_I_ORIGINABLE_H */
