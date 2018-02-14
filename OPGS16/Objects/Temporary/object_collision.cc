#include "object_collision.h"
#include "..\..\System\Shader\shader_wrapper.h"        /*! ShaderWrapper */
#include <glm\gtc\matrix_transform.hpp>

ObjectCollidable::ObjectCollidable(const std::string& sprite_tag) :
    m_sprite_renderer{ sprite_tag, "gQuad" } {}

void ObjectCollidable::Update() {
}

void ObjectCollidable::Draw() {
    auto& shader = m_sprite_renderer.GetWrapper();

    auto M = glm::mat4();
    auto position = GetFinalPosition();
    M = glm::translate(M, glm::vec3{ position.x, position.y, 0 });
    M = glm::rotate(M, glm::radians(GetRotationAngle()), GetRotationFactor());
    M = glm::scale(M, GetScaleFactor() * GetScaleValue());

    auto V = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    //auto P =

    //shader.ReplaceUniformValue<glm::mat4>()
}

