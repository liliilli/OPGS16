#ifndef OPGS16_SYSTEM_FRAME_SCENE_H
#define OPGS16_SYSTEM_FRAME_SCENE_H

/*!
 * @file scene.h
 * @brief The file consist of basic scene API.
 *
 * All derived class based on Scene class can be used parameter of Application::PushScene().
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-19 Refactoring
 * 2018-03-11 Correcton of Object to opgs16::element::CObject.
 * 2018-03-16 Fixed Instantiate to be able to return created instance's pointer.
 */

#include <memory>                           /*! std::unique_ptr<> */
#include "../Element/Public/object.h"       /*! ::opgs16::elent::CObject */
#include "../Helper/Public/template.h"
#include "../../Headers/Fwd/objectfwd.h"    /*! ::opgs16::component::CCamera */

namespace opgs16 {
namespace element {

} /*! opgs16::element */
} /*! opgs16 */

/*!
 * @class Scene
 * @brief Base scene interface.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-16 Fixed Instantiate to be able to return created instance's pointer.
 */
class Scene {
private:
    using Object = opgs16::element::CObject;
    using object_ptr = std::unique_ptr<Object>;
    using object_map = std::unordered_map<std::string, object_ptr>;
    using _camera = opgs16::component::CCamera;

public:
	/** Must need virtual dtor */
    virtual ~Scene() = default;

    virtual void Initiate() = 0;

    /**
     * @brief The method update components movement, UI refresh, and so on.
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Update() of m_object_list.
     */
	virtual void Update();

    /**
     * @brief The method calls scene to draw all m_object_list.
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Draw() of m_object_list.
     */
    virtual void Draw() {};

	/**
	 *
	 */
	template <
        class _Ty,
        typename = std::enable_if_t<IsCObjectBase<_Ty> && IsCObjectSmtPtr<_Ty>>
    >
    _Ty* Instantiate(const std::string& tag, std::unique_ptr<_Ty>&& obj) {
        if (DoesObjectExist(tag)) return nullptr;
		m_object_list[tag] = std::move(obj);
        m_object_list[tag]->SetHash(tag);
        return static_cast<_Ty*>(m_object_list[tag].get());
	}

	template <
        class _Ty,
        typename = std::enable_if_t<IsCObjectBase<_Ty> && IsCObjectSmtPtr<_Ty>>
    >
    _Ty* Instantiate(const std::string& tag, std::unique_ptr<_Ty>& obj) {
        if (DoesObjectExist(tag)) return nullptr;
		m_object_list[tag] = std::move(obj);
        m_object_list[tag]->SetHash(tag);
        return static_cast<_Ty*>(m_object_list[tag].get());
	}

    template <
        class _Ty,
        class... _Args,
        typename = std::enable_if_t<IsCObjectBase<_Ty>>
    >
    _Ty* Instantiate(const std::string& tag, _Args&&... args) {
        if (DoesObjectExist(tag)) return nullptr;
        m_object_list[tag] = std::make_unique<_Ty>(std::forward<_Args>(args)...);
        m_object_list[tag]->SetHash(tag);
        return static_cast<_Ty*>(m_object_list[tag].get());
	}

	/*!
	 * @brief Get object list loaded in scene.
	 * @return The reference of object list with hash_map.
	 */
	object_map& GetObjectList();

	/*!
	 * @brief Get specific object with tag.
	 */
	object_ptr& GetObject(const std::string& tag);

    /*!
     * @brief Set main camera of this scene, to display game scene.
     * All object except for Canvas m_object_list (UI object) uses to main_camera to display.
     * If main_camera value is nullptr, this means main_camera is detached.
     */
    inline void SetMainCamera(_camera* const main_camera);

    /*!
     * @brief Get bound main camera. if main camera is not bound, return nullptr.
     */
    inline const _camera* const GetMainCamera();

    inline bool DoesObjectExist(const std::string& name) const {
        return m_object_list.find(name) != m_object_list.end();
    }

private:
    object_map m_object_list;
    _camera* m_main_camera{ nullptr };
};

inline void Scene::SetMainCamera(_camera* const main_camera) {
    m_main_camera = main_camera;
}

inline const opgs16::component::CCamera* const Scene::GetMainCamera() {
    return m_main_camera;
}

#endif // OPGS16_SYSTEM_FRAME_SCENE_H

