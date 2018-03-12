#ifndef OPGS16_SYSTEM_MANAGER_SCENE_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_SCENE_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Manager/Public/scene_manager.h
 * @author Jongmin Yun
 *
 * @log
 * 2018-02-14 Create file.
 * 2018-03-04 Refactoring.
 */

#include <memory>
#include <stack>
#include "../../Core/Public/application.h" /*! Application for callback forwarding */
#include "../../Frame/scene.h"     /*! Scene */

namespace opgs16 {
namespace manager {

/*!
 * @class MSceneManager
 * @brief Manager class manages game scene.
 */
class MSceneManager final {
private:
    using scene_stack = std::stack<std::unique_ptr<Scene>>;

public:
    /*! Return singleton instance of SceneManager. */
    static MSceneManager& Instance() {
        static MSceneManager instance{};
        return instance;
    }

    /*!
     * @brief The method that adds scene to scene stack.
     * Add scene to scene stack stores scenes is paused, and move to top scene.
     *
     * @param[in] _Ty* Type parameter is based on Scene, value must be nullptr to prevent
     * double initiation of scene.
     */
    template <
        class _Ty,
        typename = std::enable_if_t<
        std::is_base_of_v<Scene, _Ty>
        >
    >
    void PushScene();

    /*!
     * @brief The method replace scene with top scene.
     * before actually replacing previous scene, application have to go through purify resources,
     * to prevent unintentional memory leak and runtime errors.
     */
    template <
        class _Ty,
        typename = std::enable_if_t<
        std::is_base_of_v<Scene, _Ty>
        >
    >
    void ReplaceScene() {
        auto& app = opgs16::MApplication::Instance();
        app.SetOnBeforeUpdateCallback(std::bind(&MSceneManager::PrivateReplaceScene<_Ty>, this));
    }

    /**
     * @brief The method removes top (present) scene.
     * If there is no scene, exit application automatically.
     * Otherwise all Update() and Rendering procedures delegates to previous scene.
     */
    void PopScene();

   /*!
    * @brief
    * @return
    */
    inline scene_stack& GetLoadedSceneList() noexcept { return m_scenes; }

    /*!
     * @brief Get top scene's pointer.
     * @return The pointer of top scene, if application has no scene return nullptr
     */
    inline Scene* const PresentScene();

    /*!
     * @brief
     * @return
     */
    inline bool Empty() const noexcept;

private:
    scene_stack m_scenes;	/** Scene stack */

private:
    /*! Release all resource used in scene, prevent runtime error and resource memory leak. */
    void ReleaseAllResources() const;

    template <
        class _Ty,
        typename = std::enable_if_t<
           std::is_base_of_v<Scene, _Ty>
        >
    >
    void PrivateReplaceScene() {
         /*! Purify remain resources */
         ReleaseAllResources();
         /** Pop present scene */
         m_scenes.pop();
         /** Push present scene */
         PushScene<_Ty>();
    }

private:
    MSceneManager() = default;

public:
    ~MSceneManager();
    MSceneManager(const MSceneManager&) = delete;
    MSceneManager& operator=(MSceneManager&) = delete;
};

template <class _Ty, typename>
void MSceneManager::PushScene() {
    m_scenes.push(std::make_unique<_Ty>());
    m_scenes.top()->Initiate();
}

inline Scene* const MSceneManager::PresentScene() {
    if (m_scenes.empty()) return nullptr;
    else return m_scenes.top().get();
}

inline bool MSceneManager::Empty() const noexcept {
    return m_scenes.empty();
}

} /*! opgs16::manager */
} /*! opgs16 */

#define M_REPLACE_SCENE(__scene_name__) \
opgs16::manager::SceneManager::Instance().ReplaceScene<__scene_name__>()

#endif // !OPGS16_SYSTEM_MANAGER_SCENE_MANAGER_H
