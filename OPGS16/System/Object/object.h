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
 * @date 2018-02-08
 */

#include <memory>			/** std::unique_ptr */
#include <unordered_map>	/** std::unordered_map */
#include <string>           /*! std::to_string */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! helper::ShaderNew
                                              * glm::vec3
                                              * ObjectTree
                                              * ObjectImplDeleter */

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
	virtual ~Object() = default;

	using object_raw = Object*;
	using object_ptr = std::unique_ptr<Object>;
	using object_map = std::unordered_map<std::string, object_ptr>;

    /**
     * @brief The method updates components of object.
     */
    [[noreturn]] virtual void Update();

    /**
     * @brief The method calls scene to one objects.
     * @param[in] shader Shader to use.
     */
    [[noreturn]]
	[[deprecated("Use plain-Draw method which do not have shader argument.")]]
	virtual void Draw(helper::ShaderNew& shader) {};

	/**
	 * @brief This calls callee to draw or render something it has. [Optional]
	 */
	[[noreturn]] virtual void Draw() {};

    /**
     * @brief The method gets position as glm::vec3 type. Not overridable.
     * @return Object's position (x, y, z)
     */
    const glm::vec3 GetLocalPosition() const;

    /**
     * @brief The method sets position. Not overridable.
     * @param[in] position Position to set on.
     */
    [[noreturn]] virtual void SetLocalPosition(glm::vec3 position);

	/**
	 * @brief Return final position. used for hierarchy structure.
	 * @return Object's final position in hierarchy.
	 */
	const glm::vec3 GetFinalPosition() const;

	/**
	 * @brief Set final position.
	 * @param[in] final_position Final position in Screen space.
	 */
	[[noreturn]] void SetFinalPosition(const glm::vec3& final_position);

	/**
	 * @brief The method refresh final position. Nor overriadble.
	 * @param[in] parent_position Position to apply for.
	 */
	[[noreturn]] void UpdateFinalPosition(const glm::vec3& parent_position);

    /**
     * @brief The method gets rotation values
     * @return Object's rotation angle value.
     */
    const float GetRotationAngle() const;

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
    const glm::vec3 GetRotationFactor() const ;

    /**
     * @brief The method sets rotation factor have values which ranges [-1, 1].
     * @param[in] factor
     */
    [[noreturn]] void SetRotationFactor(const glm::vec3& factor);

    /**
     * @brief The method gets scaling values
     * @return Object's scaling value.
     */
    const float GetScaleValue() const;

    /**
     * @brief The method sets scaling angle values.
     * @param[in] scale_value Scaling value to set on.
     */
    [[noreturn]] void SetScaleValue(const float scale_value);

    /**
     * @brief The method gets (x, y, z) glm::vec3 scaling axis factor.
     * @return Object's scaling vector which has (x, y, z) axis factors.
     */
    const glm::vec3 GetScaleFactor() const;

    /**
     * @brief The method sets scaling vector have (x, y, z) scaling factors.
     * @param[in] factor Scaling factor
     */
    [[noreturn]] void SetScaleFactor(const glm::vec3& factor);

    /**
     * @brief The method returns Model matrix, M = TRS
     *
     * If any value which related to Translate, Rotation, Scaling has been changed,
     * model matrix would be calculated newly and returned.
     *
     * @return Model matrix (M = TRS)
     */
    const glm::mat4 GetModelMatrix() const;

	/**
	 * @brief Set active option of object.
	 * If m_active is false, this object cannot update until m_active return to true.
	 * @param[in] value Active option value.
	 */
	[[noreturn]] void SetActive(const bool value);

	/** Get active value. */
	bool GetActiveValue();

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
	bool Instantiate(const std::string tag, _Types&&... _args) {
        const auto item_tag = CreateChildTag(tag);
		m_children[item_tag] = std::make_unique<_Ty>(std::forward<_Types>(_args)...);
		return true;
	}

	template <class _Ty, class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool Instantiate(const std::string tag, std::unique_ptr<_Ty>& instance) {
        const auto item_tag = CreateChildTag(tag);
		m_children[item_tag] = std::move(instance);
		return true;
	}

    template <class _Ty, class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
    bool Instantiate(const std::string tag) {
        const auto item_tag = CreateChildTag(tag);
		m_children[item_tag] = std::make_unique<_Ty>();
		return true;
    }

    /*!
     * @brief
     * @param[in]
     * @return
     */
    inline const std::string CreateChildTag(const std::string& tag) noexcept {
        std::string item_tag{ tag };

        if (m_tag_counter.find(tag) != m_tag_counter.end()) {
            auto& count = m_tag_counter[tag];
            count += 1;
            item_tag.append('_' + std::to_string(count));
        }
        else { m_tag_counter[tag] = 0; }

        return item_tag;
    }

	/**
	 * @brief Destroy child object has unique tag key.
	 * @param[in] tag Object tag.
	 * @return Success/Failed tag. If arbitary objects has been destroied, return ture.
	 */
	bool DestroyChild(const std::string& tag);

	/**
	 * @brief Get children tag list.
	 * @return Children's tags container of object.
	 */
	const std::vector<std::string> GetChildrenTags() const;

	/**
	 * @brief Get children reference.
	 * @return Children objects component list.
	 */
	object_map& GetChildren();

	/**
	 * @brief Get arbitary child object.
	 * @param[in] tag The tag of object to find.
	 * @return Object's smart-pointer instance.
	 */
	object_raw GetChild(const std::string& tag);

	/**
	 * @brief This only must be called by Application methods body,
	 * @returns traversal recursive object tree, to be checked in DEBUG MODE.
	 */
	[[noreturn]] void GetObjectTree(ObjectTree* const tree);

private:
	std::unique_ptr<ObjectImpl, ObjectImplDeleter> m_data{ nullptr };
	object_map m_children;

    using tag_counter_map = std::unordered_map<std::string, size_t>;
    tag_counter_map m_tag_counter;
};

#endif /** OPENGL_TUTORIAL_OBJECT_H */