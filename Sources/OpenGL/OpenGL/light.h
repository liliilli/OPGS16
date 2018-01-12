#ifndef OPENGL_TUTORIAL_CONTAINERS_H
#define OPENGL_TUTORIAL_CONTAINERS_H

#include <array>
#include <glm/glm.hpp>

namespace light {

class DirectionalLight {
public:
    /**
    * @brief 
    * @param
    */
    DirectionalLight(const glm::vec3 dir, const glm::vec3 ambient, const glm::vec3 diffuse,
                     const glm::vec3 specular);

    /**
    * @brief Set direction with new vector (x, y, z). input vector is normalized.
    * @param direction Direction vector (x, y, z)
    */
    [[noreturn]] void SetDirection(const glm::vec3 direction);
    const auto GetDirection() const { return direction; }

    /**
    * @brief Set position with new coordinate (x, y, z)
    * @param position Position (x, y, z)
    */
    [[noreturn]] void SetPosition(const glm::vec3 position);
    const auto GetPosition() const { return standard_position; }

    /**
     * @brief Refresh matrix T used for shadow mapping.
     * @return Return const reference of this. you can call GetMatrix() by succeed after this call.
     */
    const DirectionalLight& RefreshTmat();

    const auto GetAmbient() const { return ambient; }
    const auto GetDiffuse() const { return diffuse; }
    const auto GetSpecular() const { return specular; }
    const auto GetMatrix() const { return matrix_T; }


private:
    glm::vec3 direction;    // Direction in world space.

    // Color properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 standard_position;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 matrix_T;
};

class PointLight {
public:
    /**
    * @brief 
    * @param
    */
    PointLight(const glm::vec3 position, 
               const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular,
               const float linear = 0.045f, const float quadratic = 0.0075f);
    PointLight(const glm::vec3 position,
               const float ambient, const float diffuse, const float specular);

    /**
    * @brief Set position with new coordinate (x, y, z)
    * @param position Position (x, y, z)
    */
    [[noreturn]] void SetPosition(const glm::vec3 position);
    const auto GetPosition() const { return position; }
    const auto GetStandPos() const { return pos_standard; }

    const auto GetAmbient() const { return ambient; }
    const auto GetDiffuse() const { return diffuse; }
    const auto GetSpecular() const { return specular; }

    const auto GetConstant() const { return constant; }
    const auto GetLinear() const { return linear; }
    const auto GetQuadratic() const { return quadratic; }

    const auto GetMatrix() const { return matrix_T; }
    const auto GetFarPlane() const { return far; }

private:
    const float far = 25.0f;

    // Light Position in view space
    glm::vec3 position;
    glm::vec3 pos_standard;

    // Color properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Attenuation properties
    float constant;
    float linear;
    float quadratic;

    std::array<glm::mat4,6> view;
    glm::mat4 projection;
    std::array<glm::mat4,6> matrix_T;
};

struct Spotlight {
    // Light direction in view space
    glm::vec3 direction;
    glm::vec3 position;

    // Color properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Attenuation properties
    float constant;
    float linear;
    float quadratic;

    // Cutoff properties (cosine value -1 ~ 1 in one side)
    float inCutOff;
    float outCutOff;
};

}

#endif
