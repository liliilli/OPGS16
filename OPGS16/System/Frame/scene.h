#ifndef OPENGL_TUTORIAL_SCENE_H
#define OPENGL_TUTORIAL_SCENE_H

/**
* @file scene.h
* @brief The file consist of basic scene API.
*
* All derived class based on Scene class can be used parameter of Application::PushScene().
* Last updated :: 2018-02-02
*
* @author Jongmin Yun
* @version 0.0.1
*/

#include <memory>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "..\Object\object.h"
#include "..\..\Headers\Fwd\objectfwd.h"    /*! component::Camera */

/**
 * @class Scene
 * @brief Base scene interface.
 */
class Scene {
public:
	/** Must need virtual dtor */
    virtual ~Scene() = default;

    [[noreturn]] virtual void Initiate() = 0;

    /**
     * @brief The method update components movement, UI refresh, and so on.
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Update() of objects.
     */
	[[noreturn]] virtual void Update();

    /**
     * @brief The method calls scene to draw all objects.
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Draw() of objects.
     */
	[[noreturn]] virtual void Draw();

	/**
	 * @brief This only must be called by Application methods body,
	 * retuns traversal recursive object tree, to be checked in DEBUG MODE.
	 */
	[[noreturn]] virtual void GetObjectTree(ObjectTree* const tree);

	/**
	 *
	 */
	template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool Instantiate(const std::string& tag, std::unique_ptr<_Ty>&& obj) {
		if (objects.find(tag) != objects.end()) return false;
		objects[tag] = std::move(obj);
        objects[tag]->m_hash_value = std::hash<std::string>{}(tag);
		return true;
	}

	template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool Instantiate(const std::string& tag, std::unique_ptr<_Ty>& obj) {
		if (objects.find(tag) != objects.end()) return false;
		objects[tag] = std::move(obj);
        objects[tag]->m_hash_value = std::hash<std::string>{}(tag);
		return true;
	}

	/*!
	 * @brief Get object list loaded in scene.
	 * @return The reference of object list with hash_map.
	 */
	Object::object_map& GetObjects();

	/*!
	 * @brief Get specific object with tag.
	 */
	Object::object_ptr& GetObject(const std::string& tag);

    /*!
     * @brief Set main camera of this scene, to display game scene.
     * All object except for Canvas objects (UI object) uses to main_camera to display.
     * If main_camera value is nullptr, this means main_camera is detached.
     */
    inline void SetMainCamera(component::Camera* const main_camera);

    /*!
     * @brief Get bound main camera. if main camera is not bound, return nullptr.
     */
    inline const component::Camera* const GetMainCamera();

private:
    Object::object_map objects;
    component::Camera* m_main_camera{ nullptr };
};

inline void Scene::SetMainCamera(component::Camera* const main_camera) {
    m_main_camera = main_camera;
}

inline const component::Camera* const Scene::GetMainCamera() {
    return m_main_camera;
}

#endif // OPENGL_TUTORIAL_SCENE_H

