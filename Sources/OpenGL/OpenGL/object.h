#ifndef OPENGL_TUTORIAL_OBJECT_H
#define OPENGL_TUTORIAL_OBJECT_H

/**
 * @file object.h
 * @brief The file consist of basic scene API.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each objects you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 *
 * @author Jongmin Yun
 * @version 0.0.1
 */

#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "camera.h"
#include "shader.h"

/**
 * @class Object
 * @brief The class for every object to update and draw.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each objects you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 *
 * Each object can be called using Update and Draw with shader to use.
 */
class Object {
public:
    Object();
    virtual ~Object();

    /**
     * @brief The method updates components of object.
     */
    [[noreturn]] virtual void Update() = 0;

    /**
     * @brief The method calls scene to one objects.
     * @param[in] shader Shader to use.
     */
    [[noreturn]] virtual void Draw(helper::ShaderNew& shader) = 0;


    /*-----------------------------------------------------------------------*
     * @comments Bolier-plate codes! 
     */

    /**
     * @brief The method gets position as glm::vec3 type. Not overridable.
     * @return Object's position (x, y, z)
     */
    glm::vec3 GetPosition();

    /**
     * @brief The method sets position. Not overridable.
     * @param[in] position Position to set on.
     */
    [[noreturn]] void SetPosition(glm::vec3 position);

    /**
     * @brief The method gets rotation values
     * @return Object's rotation angle value.
     */
    float GetRotationAngle();

    /**
     * @brief The method sets rotation angle values.
     *
     * When input value is positive and factor is [0, 1], axis rotates clockwise.
     * input value is negative and factor is [0, 1], axis rotates counter-clockwise.
     *
     * @param[in] angle_value Angle value to set on.
     */
    [[noreturn]] void SetAngle(const float angle_value);

    /**
     * @brief The method gets (x, y, z) glm::vec3 rotation axis factor.
     * @return Object's rotation vector which has (x, y, z) rotation axis factor.
     */
    glm::vec3 GetRotationFactor();

    /**
     * @brief The method sets rotation factor have values which ranges [-1, 1].
     * @param[in] factor 
     */
    [[noreturn]] void SetRotationFactor(glm::vec3 factor);

    /**
     * @brief The method gets scaling values
     * @return Object's scaling value.
     */
    float GetScaleValue();

    /**
     * @brief The method sets scaling angle values.
     * @param[in] scale_value Scaling value to set on.
     */
    [[noreturn]] void SetScaleValue(const float scale_value);

    /**
     * @brief The method gets (x, y, z) glm::vec3 scaling axis factor.
     * @return Object's scaling vector which has (x, y, z) axis factors.
     */
    glm::vec3 GetScaleFactor();

    /**
     * @brief The method sets scaling vector have (x, y, z) scaling factors.
     * @param[in] factor Scaling factor
     */
    [[noreturn]] void SetScaleFactor(glm::vec3 factor);

    /**
     * @brief The method returns Model matrix, M = TRS
     * 
     * If any value which related to Translate, Rotation, Scaling has been changed,
     * model matrix would be calculated newly and returned.
     *
     * @return Model matrix (M = TRS)
     */
    glm::mat4 GetModelMatrix();

protected:
    /** Caution :: Dangling reference! */
    camera::Camera& camera;

    /**
     * @text values related to movement, rotation, scaling.
     */ 

    /** (x, y, z) world position */
    glm::vec3   position{};

    /** Rotation angle : Positive value is CW, Negative value is CCW */
    float       rotation_angle{};
    /** Rotation factor is (x, y, z) factor did not exceed [-1, 1] range. */
    glm::vec3   rotation_factor{ 0.0f };

    /** Scale value's default value is 1.0f */
    float       scale_value{ 1.0f };
    /** Scale factor is (x, y, z) factor default is (1.0f, 1.0f, 1.0f) */
    glm::vec3   scale_factor{ 1.0f };

    glm::mat4   model;
    glm::mat4   translate;
    glm::mat4   rotate;
    glm::mat4   scale;

    bool is_changed{ true };

private:
    /**
     * @brief Refresh Translation matrix
     */
    [[noreturn]] void RefreshTranslateMatrix();

    /**
     * @brief Refresh Rotation matrix
     */
    [[noreturn]] void RefreshRotateMatrix();

    /**
     * @brief Refresh Scaling matrix
     */
    [[noreturn]] void RefreshScaleMatrix();
};

#endif /** OPENGL_TUTORIAL_OBJECT_H */