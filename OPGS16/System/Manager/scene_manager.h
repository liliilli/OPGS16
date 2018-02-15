#ifndef OPGS16_SYSTEM_MANAGER_SCENE_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_SCENE_MANAGER_H

/*!
 * @file System\Manager\scene_manager.h
 * @author Jongmin Yun
 * @date 2018-02-14
 */

#include <memory>
#include <stack>
#include "..\Frame\scene.h" /*! Scene */

/*!
 * @class SceneManager
 * @brief
 */
class SceneManager final {
private:
    using scene_stack = std::stack<std::unique_ptr<Scene>>;

public:
    /** * @brief Static method gets unique instance of SceneManager class.  */
    static SceneManager& GetInstance() {
        static SceneManager instance{};
        return instance;
    }

    /**
     * @brief The method that adds scene to scene stack.
     * Add scene to scene stack stores scenes is paused, and move to top scene.
     *
     * @param[in] _Ty* Type parameter is based on Scene, value must be nullptr to prevent
     * double initiation of scene.
     */
    template <class _Ty, typename = std::enable_if_t<std::is_base_of_v<Scene, _Ty>>>
    [[noreturn]] void PushScene();

    /**
     * @brief The method replace scene with top scene.
     */
    template <class _Ty, typename = std::enable_if_t<std::is_base_of<Scene, _Ty>::value>>
    [[noreturn]] void ReplaceScene(){
		pReplaceScene<_Ty>();
    }

    /**
     * @brief The method removes top (present) scene.
	 * If there is no scene, exit application automatically.
	 * Otherwise all Update() and Rendering procedures delegates to previous scene.
     */
    [[noreturn]] void PopScene();

    /*!
     * @brief
     * @return
     */
    inline scene_stack& GetLoadedSceneList() noexcept { return m_scenes; }

    /*!
     * @brief Get top scene's pointer.
     * @return The pointer of top scene, if application has no scene return nullptr
     */
    inline Scene* const GetPresentScene();

    /*!
     * @brief
     * @return
     */
    inline bool SceneEmpty() const noexcept;

private:
    scene_stack m_scenes;	/** Scene stack */

private:
   	/**
	* @brief The method replace scene with old scene.
	*/
	template <class _Ty, typename = std::enable_if_t<std::is_base_of<Scene, _Ty>::value>>
	[[noreturn]] void pReplaceScene() {
		/** Pop present scene */
		m_scenes.pop();
		/** Push present scene */
		PushScene<_Ty>();
	}
};

template <class _Ty, typename>
void SceneManager::PushScene() {
    m_scenes.push(std::make_unique<_Ty>());
    m_scenes.top()->Initiate();
}

inline Scene* const SceneManager::GetPresentScene() {
    if (m_scenes.empty()) return nullptr;
    else return m_scenes.top().get();
}

inline bool SceneManager::SceneEmpty() const noexcept {
    return m_scenes.empty();
}

#endif // !OPGS16_SYSTEM_MANAGER_SCENE_MANAGER_H
