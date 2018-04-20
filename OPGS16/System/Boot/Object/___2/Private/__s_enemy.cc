
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Boot/___2/Public/__s_enemy.h
 * @brief Enemy object class.
 * @log
 * 2018-04-19 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_enemy.h"    /*! Header file */

#include "../../../../Manager/Public/scene_manager.h"       /*! ::opgs16::manager::SceneManager */
#include "../../../../Components/Public/camera.h"           /*! ::opgs16::component::Camera */
#include "../../../../Shader/shader_wrapper.h"

#include "../../../../Components/Public/sprite_renderer.h"  /*! ::opgs16::component::CSprite2DRenderer */
#include "../../../Scripts/___2/Public/__s_script_enemy.h"  /*! ::opgs16::*/
#include "../Interface/constants.h"

namespace opgs16::builtin::sample {

__S_ENEMY::__S_ENEMY(CObject* parent) : m_parent{ *parent } {
    SetScaleValue(12.f);
    SetWorldPosition({ 0, -k_enemy_circle_radius, 0 });
    SetRotationLocalAngle(element::_internal::EDirection::X, -10.f);

    m_renderer = AddComponent<component::CSprite2DRenderer>(*this, "System", "gQuad");
    m_renderer->SetTextureIndex(10);
    AddComponent<__S_SCRIPT_ENEMY>(*this);
}

void __S_ENEMY::Render() {
    if (m_renderer) {
        using manager::MSceneManager;
        using component::CSprite2DRenderer;

        const float angle = m_parent.GetRotationWorldAngle(element::_internal::EDirection::Z) * k_pi180;
        const float end_z = (GetFinalPosition().z - k_end) / k_z_length;
        const float x = (std::sinf(angle) * k_enemy_circle_radius) * end_z;
        const float y = (std::cosf(angle) * -k_enemy_circle_radius) * end_z;

        auto model_matrix = GetModelMatrix();
        model_matrix[0] *= end_z;
        model_matrix[1] *= end_z;
        model_matrix[2] *= end_z;
        model_matrix[3][0] = k_center_pos[0] + x;
        model_matrix[3][1] = k_center_pos[1] + y;

        const auto pvm = MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * model_matrix;
        m_renderer->Wrapper().SetUniformValue<glm::mat4>("projection", pvm);
        m_renderer->RenderSprite();
    }
};

} /*! opgs16::builtin::sample */
