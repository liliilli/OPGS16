#ifndef OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_OBJECT_AXIS_H
#define OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_OBJECT_AXIS_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/___2/Public/__s_object_axis.h
 * @brief Object axis file binding all moveable object except for UI objects.
 * @log
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../../../../Components/Public/sprite_renderer.h"  /*! ::opgs16::component::CSprite2DRenderer */
#include "../../../../Components/Public/camera.h"       /*! ::ogps16::component::CCamera */
#include "../../../../Element/Public/object.h"          /*! ::ogps16::element::CObject */
#include "../../../../Element/Public/scene.h"           /*! ::opgs16::element::CScene */
#include "../../../../Manager/Public/scene_manager.h"   /*! ::opgs16::manager::MSceneManager */
#include "../../../../Shader/shader_wrapper.h"          /*! ::opgs16::element::CShaderWrapper */
#include "../../../../Helper/Public/template.h"         /*! template SFINAE helper functions */

namespace opgs16 {
namespace builtin {
namespace sample {

template <class _Ty, typename = std::enable_if_t<IsCObjectBase<_Ty>>>
class __S_OBJECT_AXIS final : public element::CObject {
public:
    __S_OBJECT_AXIS() : m_object{ Instantiate<_Ty>("Object") } {
        m_renderer = m_object->GetComponent<component::CSprite2DRenderer>();
    }

private:
    constexpr float k_end{ 100.f };
    constexpr float k_start{ 0.f };

    component::CSprite2DRenderer* m_renderer{ nullptr };
    CObject* m_object{ nullptr };

    void Render() override final {
        if (m_renderer) { // Render bound object.
            glm::mat4 model_matrix = GetModelMatrix();
            // Write algorithm.

            // End of algorithm.
            const auto pvm = manager::MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * model_matrix;
            m_renderer->Wrapper().SetUniformValue<glm::mat4>("projection", pvm);
            m_renderer->RenderSprite();
        }
    }
};

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! ogps16 */

#endif // OPGS16_SYSTEM_BOOT_OBJECT____2_PUBLIC___S_OBJECT_AXIS_H