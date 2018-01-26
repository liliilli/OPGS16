#include "i_originable.h"

#include <array>
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

inline void IOriginable::SetOrigin(Origin value) {
	origin = value;
}

glm::vec2 IOriginable::CalculateCenterPosition(const Origin & origin, const glm::vec2 & position) const {
	std::array<GLint, 4> viewport{};
	glGetIntegerv(GL_VIEWPORT, &viewport[0]);

	auto origin_type = static_cast<int>(origin) - 1;
	auto y_value = origin_type / 3;
	auto x_value = origin_type % 3;

	GLint x_pos = viewport[2] * x_value / 2;
	GLint y_pos = viewport[3] * y_value / 2;

	return glm::vec2{ x_pos + position.x, y_pos + position.y };
}
