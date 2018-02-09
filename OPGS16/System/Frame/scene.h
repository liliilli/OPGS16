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

/**
 * @class Scene
 * @brief Base scene interface.
 */
class Scene {
public:
	/** Must need virtual dtor */
    virtual ~Scene() = default;

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
	bool InsertObject(const std::string& tag, std::unique_ptr<_Ty>&& obj) {
		if (objects.find(tag) != objects.end()) return false;
		objects[tag] = std::move(obj);
		return true;
	}

	template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Object, _Ty>>>
	bool InsertObject(const std::string& tag, std::unique_ptr<_Ty>& obj) {
		if (objects.find(tag) != objects.end()) return false;
		objects[tag] = std::move(obj);
		return true;
	}

	/**
	 * @brief
	 *
	 * @return
	 */
	Object::object_map& GetObjects();

	/**
	 *
	 */
	Object::object_ptr& GetObject(const std::string& tag);

private:
    Object::object_map objects;
};

#endif // OPENGL_TUTORIAL_SCENE_H

