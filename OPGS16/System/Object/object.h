#ifndef OPGS16_SYSTEM_OBJECT_OBJECT_H
#define OPGS16_SYSTEM_OBJECT_OBJECT_H

/**
 * @file System\Object\object.h
 * @brief The file consist of basic scene API.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each m_object_list you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 *
 * @author Jongmin Yun
 * @date 2018-02-19
 *
 * @log
 * 2018-02-19 Refactoring. Remove Draw(ShaderNew) obsolete not helpful method. Yeah!
 * 2018-02-23 Add hierarchy rotation and scaling option.
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

/**
 * @class Object
 * @brief The class for every object to update and draw.
 *
 * Object abstract class stores common values and inheritable by derived each object class.
 * When using each m_object_list you have to use this type as a storing type to avoid cracking of
 * polymorphism.
 * Each object can be called using Update and Draw with shader to use.
 * @date 2018-02-19
 *
 * @log
 * 2018-02-19 Refactoring. Remove Draw(ShaderNew) obsolete not helpful method. Yeah!
 * 2018-02-19 Add GetParentPosition(). Remove virtual property from Draw()
 *            and Render() virtual methods. and Remove virtual property from Update() instead of
 *            adding LocalUpdate() method which replaces Update() override.
 */
class Object {
private:
    using component_ptr     = std::unique_ptr<component::Component>;
    using component_list    = std::vector<component_ptr>;
    using name_counter_map  = std::unordered_map<std::string, size_t>;
	using object_raw = Object*;
	using object_ptr = std::unique_ptr<Object>;
	using object_map = std::unordered_map<std::string, object_ptr>;
    using pimpl_ptr  = std::unique_ptr<ObjectImpl, ObjectImplDeleter>;

public:
	Object();
	virtual ~Object() = default;

    /*! Update components of object. */
    void Update() {
        if (m_data && GetActive()) {
            LocalUpdate();
            for (auto& component : m_components)
                component->Update();

            for (auto& child : m_children) {
                /*! If child.second is not emtpy and activated. */
                if (child.second && child.second->GetActive()) {
                    child.second->SetParentPosition(GetParentPosition());
                    child.second->Update();
                }
            }
        }
    }

	/*! Calls children to draw or render something it has.  */
    void Draw() {
        Render();
        /*! Order draw call to exist child object. */
        for (auto& object : m_children) {
            if (object.second) object.second->Draw();
        }
    }

    /*! This method will be called when Collision. */
    virtual void OnCollisionEnter(component::Rigidbody2D& collider) {};

    /*! This method will be called when Trigger entered. */
    virtual void OnTriggerEnter(component::Rigidbody2D& collider) {};

    /*!
     * @brief Return local position.
     * @return Object's local position.
     */
    const glm::vec3& GetLocalPosition() const noexcept;

    /*!
     * @brief Sets local position.
     * @param[in] local position Position to set on.
     */
    void SetLocalPosition(const glm::vec3& position) noexcept;

	/*!
	 * @brief Return world position.
	 * @return Object's world position from parent object's position.
	 */
	const glm::vec3& GetWorldPosition() const noexcept;

	/**
	 * @brief Set world position.
	 * @param[in] world_position Winal position in Screen space and from parent' object.
	 */
	void SetWorldPosition(const glm::vec3& world_position);

	/**
	 * @brief The method refresh parent position.
	 * @param[in] parent_position Position to apply for.
	 */
	void SetParentPosition(const glm::vec3& parent_position);

    /*! Get Object's final position. */
    const glm::vec3& GetFinalPosition() const noexcept;

    /**
     * @brief The method gets rotation angle value
     * @return Object's rotation angle value.
     */
    const float GetRotationAngle() const noexcept;

    /**
     * @brief The method sets rotation angle values.
     * When input value is positive and factor is [0, 1], axis rotates clockwise.
     * input value is negative and factor is [0, 1], axis rotates counter-clockwise.
     * @param[in] angle_value Angle value to set on.
     */
    void SetRotationAngle(const float angle_value);

    /**
     * @brief The method gets (x, y, z) glm::vec3 rotation axis factor.
     * @return Object's rotation vector which has (x, y, z) rotation axis factor.
     */
    const glm::vec3& GetRotationFactor() const noexcept;

    /**
     * @brief The method sets rotation factor have values which ranges [-1, 1].
     * @param[in] factor
     */
    void SetRotationFactor(const glm::vec3& factor);

    /**
     * @brief The method gets scaling values
     * @return Object's scaling value.
     */
    const float GetScaleValue() const noexcept;

    /**
     * @brief The method sets scaling angle values.
     * @param[in] scale_value Scaling value to set on.
     */
    void SetScaleValue(const float scale_value);

    /**
     * @brief The method gets (x, y, z) glm::vec3 scaling axis factor.
     * @return Object's scaling vector which has (x, y, z) axis factors.
     */
    const glm::vec3& GetScaleFactor() const noexcept;

    /**
     * @brief The method sets scaling vector have (x, y, z) scaling factors.
     * @param[in] factor Scaling factor
     */
    void SetScaleFactor(const glm::vec3& factor);

    /**
     * @brief The method returns Model matrix, M = TRS
     *
     * If any value which related to Translate, Rotation, Scaling has been changed,
     * model matrix would be calculated newly and returned.
     *
     * @return Model matrix (M = TRS)
     */
    const glm::mat4& GetModelMatrix() const;

    void SetSucceedingPositionFlag(bool value) noexcept;

    void SetSucceedingRotationFlag(bool value) noexcept;

    void SetSucceedingScalingFlag(bool value) noexcept;

    bool GetSucceedingPositionFlag() const noexcept;

    bool GetSucceedingRotationFlag() const noexcept;

    bool GetSucceedingScalingFlag() const noexcept;

	/**
	 * @brief Set active option of object.
	 * If m_active is false, this object cannot update until m_active return to true.
	 * @param[in] value Active option value.
	 */
	void SetActive(const bool value);

	bool GetActive();   /*! Get active value. */

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
    >   bool Instantiate(const std::string name, _Types&&... _args);

    /*! Overloaded function of Instantiate(Varadic...) */
    template <
        class _Ty,
        class = std::enable_if_t<std::is_base_of_v<Object, _Ty>>
    >   bool Instantiate(const std::string name, std::unique_ptr<_Ty>& instance);

	/**
	 * @brief Destroy child object has unique tag key.
	 * @param[in] name Object name.
	 * @return Success/Failed tag. If arbitary m_object_list has been destroied, return ture.
	 */
	bool DestroyChild(const std::string& name);

	/**
	 * @brief Get children tag list.
	 * @return Children's tags container of object.
	 */
	std::vector<std::string> GetChildrenNameList() const;

	/**
	 * @brief Get children reference.
	 * @return Children m_object_list component list.
	 */
	object_map& GetChildList();

	/**
	 * @brief Get arbitary child object.
	 * @param[in] child_name The name of object to find.
	 * @return Object's raw-pointer instance. this cannot removeable.
	 */
	object_raw const GetChild(const std::string& child_name);

	/**
	 * @brief This only must be called by Application methods body,
	 * @returns traversal recursive object tree, to be checked in DEBUG MODE.
	 */
	 void GetObjectTree(ObjectTree* const tree);

    /*!
     * @brief Add component and bind to this object instance.
     * @param[in] _Ty Component type class argument.
     * @param[in] _Params&& Universal reference. Used to be arguments of Component constructor.
     */
    template<
        class _Ty,
        typename... _Params,
        typename = std::enable_if_t<std::is_base_of_v<component::Component, _Ty>>
    >   void AddComponent(_Params&&... params);

    /*!
     * @brief Return component raw-pointer.
     * @param[in] _Ty Component type argument.
     * @return If found, return _Ty* but not found, return nullptr.
     */
    template<
        class _Ty,
        typename = std::enable_if_t<std::is_base_of_v<component::Component, _Ty>>
    >   _Ty* const GetComponent();

    /*!
     * @brief Remove component.
     * @param[in] _Ty Component type argument.
     * @return If found, return true but otherwise false.
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
     void SetTag(const std::string& tag_name);

     /*! Overloaded version of SetTag(tag_name) */
     void SetTag(const size_t tag_index);

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

    /*! Return hash value of this object. */
    inline size_t GetHash() const {
        return m_hash_value;
    }

    /*! Set hash value */
    inline void SetHash(const std::string& name) const {
        if (!m_hash_initialized) {
            m_object_name = name;
            m_hash_value = std::hash<std::string>{}(name);
            m_hash_initialized = true;
        }
    }

    /*! Return object name */
    inline const std::string& GetObjectName() const {
        return m_object_name;
    }

private:
    name_counter_map m_name_counter;    /*! Object name counter to avoid duplicated object name */

    mutable std::string m_object_name{};        /*! this object name */
    mutable size_t m_hash_value{};              /*! Hash value to verify object number */
    mutable bool m_hash_initialized{ false };   /*! Flag */

protected:
	pimpl_ptr   m_data{ nullptr };              /*! Pointer implementation heap instance. */
	object_map  m_children;                     /*! The container stores child object. */
    component_list m_components{};              /*! Component list of thie object. */

private:
    /*!
     * @brief Create child object name.
     * @param[in] name
     * @return
     */
    inline const std::string CreateChildTag(const std::string& name) noexcept;

    /*! Return object final position not included local position. */
    const glm::vec3& GetParentPosition() const noexcept;

protected:
    /*! Local update method for derived object. */
    virtual void LocalUpdate() {};

    /*! Render method for derived object. */
    virtual void Render() {};
};

template <class _Ty, class... _Types, typename>
bool Object::Instantiate(const std::string tag, _Types&&... _args) {
    const auto item_tag = CreateChildTag(tag);
    m_children.emplace(item_tag, std::make_unique<_Ty>(std::forward<_Types>(_args)...));
    m_children[item_tag]->SetHash(item_tag);
    return true;
}

template <class _Ty, typename>
bool Object::Instantiate(const std::string tag, std::unique_ptr<_Ty>& instance) {
    const auto item_tag = CreateChildTag(tag);
    m_children[item_tag] = std::move(instance);
    m_children[item_tag]->SetHash(item_tag);
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