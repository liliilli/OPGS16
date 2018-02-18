#ifndef OPGS16_SYSTEM_OBJECT_OBJECT_H
#define OPGS16_SYSTEM_OBJECT_OBJECT_H

/**
 * @file System\Object\object.h
 * @brief The file consist of basic scene API.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each objects you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 *
 * @author Jongmin Yun
 * @date 2018-02-17
 */

#include <algorithm>        /*! std::find_if */
#include <functional>       /*! std::hash */
#include <memory>			/** std::unique_ptr */
#include <unordered_map>	/** std::unordered_map */
#include <string>           /*! std::to_string */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! helper::ShaderNew
                                              * glm::vec3
                                              * component::Component
                                              * component::Rigidbody2D
                                              * ObjectTree
                                              * ObjectImplDeleter
                                              */
#include "..\..\System\Components\component.h"   /*! component::Component */
//#include "..\Components\Physics2D\rigidbody_2d.h"   /*! Rigid*/

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
    using name_counter_map = std::unordered_map<std::string, size_t>;
    using component_ptr = std::unique_ptr<component::Component>;

    /**
     * @brief The method updates components of object.
     */
    [[noreturn]] virtual void Update();

    /**
     * @brief [DEPRECATED] The method calls scene to one objects.
     * @param[in] shader Shader to use.
     */
    [[noreturn]]
	[[deprecated("Use plain-Draw method which do not have shader argument.")]]
	virtual void Draw(helper::ShaderNew& shader) {};

	/** * @brief This calls callee to draw or render something it has.  */
    [[noreturn]] virtual void Draw() {
        for (auto& object : m_children) {
            if (object.second) object.second->Draw();
        }
    };

    /*!  * @brief This method will be called when Collision.  */
    [[noreturn]] virtual void OnCollisionEnter(component::Rigidbody2D& collider) {};

    /*!  * @brief This method will be called when Trigger entered.  */
    [[noreturn]] virtual void OnTriggerEnter(component::Rigidbody2D& collider) {};

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
	const glm::vec3 GetWorldPosition() const;

	/**
	 * @brief Set final position.
	 * @param[in] final_position Final position in Screen space.
	 */
	[[noreturn]] void SetWorldPosition(const glm::vec3& final_position);

	/**
	 * @brief The method refresh final position. Nor overriadble.
	 * @param[in] parent_position Position to apply for.
	 */
	[[noreturn]] void SetParentPosition(const glm::vec3& parent_position);

    /*! Get Object's final position. */
    const glm::vec3 GetFinalPosition() const;

    /**
     * @brief The method gets rotation values
     * @return Object's rotation angle value.
     */
    const float GetAngle() const;

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
	bool GetActive();

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
    template <
        class _Ty,
        class... _Types,
        class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>
    >   bool Instantiate(const std::string tag, _Types&&... _args);

    template <
        class _Ty,
        class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>
    >   bool Instantiate(const std::string tag, std::unique_ptr<_Ty>& instance);

    template <
        class _Ty,
        class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>
    >   bool Instantiate(const std::string tag);

	/**
	 * @brief Destroy child object has unique tag key.
	 * @param[in] name Object name.
	 * @return Success/Failed tag. If arbitary objects has been destroied, return ture.
	 */
	bool DestroyChild(const std::string& name);

	/**
	 * @brief Get children tag list.
	 * @return Children's tags container of object.
	 */
	const std::vector<std::string> GetChildrenNameList() const;

	/**
	 * @brief Get children reference.
	 * @return Children objects component list.
	 */
	object_map& GetChildren();

	/**
	 * @brief Get arbitary child object.
	 * @param[in] name The name of object to find.
	 * @return Object's smart-pointer instance.
	 */
	object_raw GetChild(const std::string& name);

	/**
	 * @brief This only must be called by Application methods body,
	 * @returns traversal recursive object tree, to be checked in DEBUG MODE.
	 */
	[[noreturn]] void GetObjectTree(ObjectTree* const tree);

    /*!
     * @brief
     *
     * @param[in] _Ty
     * @param[in] _Params&& Universal reference.
     */
    template<
        class _Ty,
        typename... _Params,
        typename = std::enable_if_t<std::is_base_of_v<component::Component, _Ty>>
    >   [[noreturn]] void AddComponent(_Params&&... params);

    /*!
     * @brief
     * @param[in] _Ty
     * @return
     */
    template<
        class _Ty,
        typename = std::enable_if_t<std::is_base_of_v<component::Component, _Ty>>
    >   _Ty* const GetComponent();

    /*!
     * @brief
     * @param[in] _Ty
     * @return
     */
    template <
        class _Ty,
        typename = std::enable_if_t<std::is_base_of_v<component::Component, _Ty>>
    >   bool RemoveComponent();

    /*!
     * @brief Set tag with tag name. This method will check whether or not exist matched tag name
     * in SettingManager. If not exist, do nothing and chagne error flag.
     * @param[in] tag_name Tag name
     */
    [[noreturn]] void SetTag(const std::string tag_name);

    /*! Overloading version of SetTag(tag_name) */
    [[noreturn]] void SetTag(const size_t tag_index);

    /*!
     * @brief Get tag index of this object.
     * @return Tag index value.
     */
    size_t GetTagIndexOf() const;

    /*!
     * @brief Get Tag Name of this object. (different with name of object)
     * This methods return tag name by referencing SettingManager in body.
     * @return Tag name string.
     */
    std::string GetTagNameOf() const;

    /*!
     * @brief Get Hash value()
     */
    size_t GetHash() const { return m_hash_value; }

    size_t m_hash_value;
private:
    name_counter_map m_name_counter;

protected:
	std::unique_ptr<ObjectImpl, ObjectImplDeleter> m_data{ nullptr };

	object_map m_children;

    std::vector<component_ptr> m_components{};

private:
    /*!
     * @brief
     * @param[in]
     * @return
     */
    inline const std::string CreateChildTag(const std::string& tag) noexcept;
};

template <class _Ty, class... _Types, typename>
bool Object::Instantiate(const std::string tag, _Types&&... _args) {
    const auto item_tag = CreateChildTag(tag);
    m_children[item_tag] = std::make_unique<_Ty>(std::forward<_Types>(_args)...);
    m_children[item_tag]->m_hash_value = std::hash<std::string>{}(item_tag);
    return true;
}

template <class _Ty, typename>
bool Object::Instantiate(const std::string tag, std::unique_ptr<_Ty>& instance) {
    const auto item_tag = CreateChildTag(tag);
    m_children[item_tag] = std::move(instance);
    m_children[item_tag]->m_hash_value = std::hash<std::string>{}(item_tag);
    return true;
}

template <class _Ty, typename>
bool Object::Instantiate(const std::string tag) {
    const auto item_tag = CreateChildTag(tag);
    m_children[item_tag] = std::make_unique<_Ty>();
    m_children[item_tag]->m_hash_value = std::hash<std::string>{}(item_tag);
    return true;
}

template<class _Ty, typename... _Params, typename>
void Object::AddComponent(_Params&&... params) {
    m_components.emplace_back(std::make_unique<_Ty>(std::forward<_Params>(params)...));
}

template<class _Ty, typename>
_Ty* const Object::GetComponent() {
    for (auto& item : m_components) {
        if (item->DoesTypeMatch(_Ty::type)) return static_cast<_Ty*>(item.get());
    }
    return nullptr;
}

template <class _Ty, typename>
bool Object::RemoveComponent() {
    auto it = std::find_if(m_components.cbegin(), m_components.cend(),
                           [](const std::unique_ptr<component::Component>& item) {
        return item->DoesTypeMatch(_Ty::type);
    });
    if (it != m_components.cend()) {
        m_components.erase(it);    /*! Too much execution time */
        return true;
    }
    else return false;
}

inline const std::string Object::CreateChildTag(const std::string& tag) noexcept {
    std::string item_tag{ tag };

    if (m_name_counter.find(tag) != m_name_counter.end()) {
        auto& count = m_name_counter[tag];
        count += 1;
        item_tag.append('_' + std::to_string(count));
    }
    else { m_name_counter[tag] = 0; }

    return item_tag;
}

#endif /** OPGS16_SYSTEM_OBJECT_OBJECT_H */