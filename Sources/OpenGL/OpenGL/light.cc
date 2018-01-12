#include "light.h"
#include <glm\gtc\matrix_transform.hpp>

namespace light {

DirectionalLight::DirectionalLight(const glm::vec3 dir,
                                   const glm::vec3 ambient,
                                   const glm::vec3 diffuse,
                                   const glm::vec3 specular)
    : direction{ glm::normalize(dir) },
    ambient{ ambient }, diffuse{ diffuse }, specular{ specular },
    standard_position{ 0 }, view{ 0 }, projection{ 0 }, matrix_T{ projection * view } {}

void DirectionalLight::SetDirection(const glm::vec3 direction) {
    this->direction = glm::normalize(direction);
}

void DirectionalLight::SetPosition(const glm::vec3 position) {
    standard_position = position;
}

const DirectionalLight& DirectionalLight::RefreshTmat() {
    view = glm::lookAt(standard_position, direction, glm::vec3(0, 1, 0));
    projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 12.5f);

    matrix_T = projection * view;

    return *this;
};


PointLight::PointLight(const glm::vec3 position,
                       const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular,
                       const float linear, const float quadratic)
    : position{ position }, pos_standard{ position },
    ambient{ ambient }, diffuse{ diffuse }, specular{ specular },
    constant{ 1.0f }, linear{ linear }, quadratic{ quadratic },
    projection{ glm::perspective(glm::radians(90.f), 1.0f, 1.0f, far) } {

    std::array<glm::vec3, 6> direction = {
        glm::vec3{1, 0, 0}, glm::vec3{-1,0, 0}, glm::vec3{0, 1, 0},
        glm::vec3{0,-1, 0}, glm::vec3{0, 0, 1}, glm::vec3{0, 0,-1}
    };

    std::array<glm::vec3, 6> y_pos = {
        glm::vec3{0,-1, 0}, glm::vec3{0,-1, 0}, glm::vec3{0, 0, 1},
        glm::vec3{0, 0,-1}, glm::vec3{0,-1, 0}, glm::vec3{0,-1, 0}
    };

    for (size_t i = 0; i < view.size(); ++i) {
        view[i] = glm::lookAt(position, position + direction[i], y_pos[i]);
        matrix_T[i] = projection * view[i];
    }
}
PointLight::PointLight(const glm::vec3 position, 
                       const float ambient, const float diffuse, const float specular) 
    : PointLight{ position, glm::vec3(ambient), glm::vec3(diffuse), glm::vec3(specular) } {}

void PointLight::SetPosition(const glm::vec3 position) {
    this->position = position;
};



}
