#ifndef OPGS16_SYSTEM_ELEMENT_PUBLIC_SCENE_H
#define OPGS16_SYSTEM_ELEMENT_PUBLIC_SCENE_H

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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file Element/scene.h
 * @brief The file consist of basic scene API.
 *
 * All derived class based on Scene class can be used parameter of Application::PushScene().
 *
 * @author Jongmin Yun
 * @log
 * 2018-02-19 Refactoring
 * 2018-03-11 Correcton of Object to opgs16::element::CObject.
 * 2018-03-16 Fixed Instantiate to be able to return created instance's pointer.
 * 2018-04-14 Add background color variable, and function.
 * 2018-04-14 Move ::Scene class to ::opgs16::element::Scene class. Revised DEFINE name.
 * 2018-04-14 Rename ::opgs16::element::Scene to ::opgs16::element::CScene.
 * 2018-04-14 Move file to /System/Element/Public/ directory.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <memory>                           /*! std::unique_ptr<> */

#include <GL/glew.h>

#include <Element\object.h> /// ::opgs16::element::CObject

#include <Helper\template.h>/// Type checking templates
#include <opgs16fwd.h>      /// Forward declaration

namespace opgs16 {
namespace element {

/*!
 * @class CScene
 * @brief Base scene interface.
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-16 Fixed Instantiate to be able to return created instance's pointer.
 * 2018-04-14 Add background color variable, and function.
 * 2018-04-14 Move ::Scene class to ::opgs16::element::Scene class. Revised DEFINE name.
 */
class CScene {
private:
    using object_ptr    = std::unique_ptr<CObject>;
    using object_map    = std::unordered_map<std::string, object_ptr>;
    using _camera       = component::CCamera;

public:
	/** Must need virtual dtor */
    virtual ~CScene() = default;

    virtual void Initiate() = 0;

    /**
     * @brief The method update components movement, UI refresh, and so on.
	 * This method is able to overriding,
	 * but actual default behavior is just call ->Update() of m_object_list.
     */
	virtual void Update();

    /*! Draw scene */
    void Draw() {
        glClearColor(m_background_color.r, m_background_color.g, m_background_color.b, m_background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

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
    void SetMainCamera(_camera* const main_camera) {
        m_main_camera = main_camera;
    }

    /*! Get bound main camera. if main camera is not bound, return nullptr. */
    const _camera* const GetMainCamera() {
        return m_main_camera;
    }

    bool DoesObjectExist(const std::string& name) const {
        return m_object_list.find(name) != m_object_list.end();
    }

    /*!
     * @brief Get pointer of background color.
     * @return Pointer of background color.
     */
    glm::vec4* BackgroundColor() noexcept {
        return &m_background_color;
    }

private:
    _camera*    m_main_camera{ nullptr };
    glm::vec4   m_background_color{0, 0, 0, 1};
    object_map  m_object_list;
};

} /*! opgs16::element */
} /*! opgs16 */

#endif // OPGS16_SYSTEM_ELEMENT_PUBLIC_SCENE_H

