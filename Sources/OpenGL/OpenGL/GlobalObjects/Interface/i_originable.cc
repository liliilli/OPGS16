#include "i_originable.h"

#include <array>
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>

void IOriginable::SetOrigin(Origin value) {
	origin = value;
}