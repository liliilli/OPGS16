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

#include <memory>
#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "..\..\GlobalObjects\camera.h"
#include "..\Shader\shader.h"

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

	/**
	 * @brief This calls callee to draw or render something it has. [Optional]
	 */
	[[noreturn]] virtual void Draw() {};

    /*-----------------------------------------------------------------------*
     * @comments Bolier-plate codes!
     */

    /**
     * @brief The method gets position as glm::vec3 type. Not overridable.
     * @return Object's position (x, y, z)
     */
    virtual glm::vec3 GetPosition();

    /**
     * @brief The method sets position. Not overridable.
     * @param[in] position Position to set on.
     */
    [[noreturn]] virtual void SetPosition(glm::vec3 position);

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

	/**
	 * @brief This initiate object as a child of base object.
	 *
	 * This method create & initiate object as a child, make a leaf of object hierarchy tree.
	 * Created type instance is implicitly converted (up-cast) to Object class type.
	 *
	 * If you must initiate specific derived class with initialization parameters,
	 * you can type them _args parameter. This method gets variadic inputs initialized with
	 * appropriate c-tor.
	 *
	 * You have to <> parenthesis to input specific class type to create.
	 *
	 * @param[in] object Object instance to make.
	 * @param[in] tag Object Tag.
	 * @param[in] _args variadic args to be used c-tor initialize parameters inputs.
	 * @return Success/Failed flag. If the methods success to make child object, return true.
	 */
	template <class _Ty, class... _Types, class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool InitiateChild(const std::string&& tag, _Types&&... _args) {
		if (children.find(tag) != children.end()) return false;

		children[tag] = std::make_shared<_Ty>(std::forward<_Types>(_args)...);
		return true;
	}

	template <class _Ty, class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool InitiateChild(const std::string&& tag, const _Ty&& instance) {
		if (children.find(tag) != children.end()) return false;

		children[tag] = std::make_shared<_Ty>(std::move(instance));
		return true;
	}

	/**
	 * @brief Destroy child object has unique tag key.
	 * @param[in] tag Object tag.
	 * @return Success/Failed tag. If arbitary objects has been destroied, return ture.
	 */
	bool DestroyChild(const std::string& tag);

	/**
	 * @brief Get children tag list.
	 */
	const std::vector<std::string> GetChildrenTags() const;

	/**
	 * @brief Get children reference.
	 * @return Children objects component list.
	 */
	std::unordered_map<std::string, std::shared_ptr<Object>>& GetChildren();

	/**
	 * @brief Get arbitary child object.
	 * @param[in] tag The tag of object to find.
	 * @return Object's smart-pointer instance.
	 */
	std::shared_ptr<Object> GetChild(const std::string& tag);

protected:
    /** Caution :: Dangling reference! */
    camera::Camera& camera;
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

private:
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

	std::unordered_map<std::string, std::shared_ptr<Object>> children;
};

#endif /** OPENGL_TUTORIAL_OBJECT_H */