
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Boot/Object/___2/Private/__s_background.cc
*
* @log
* 2018-04-29 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_background.h"   /*! Header file */

#include "../../../../Components/Public/camera.h"           /*! ::opgs16::component::Camera */
#include "../../../../Components/Public/sprite_renderer.h"  /*! ::opgs16::component::CSprite2DRenderer */
#include "../../../../Manager/Public/scene_manager.h"       /*! ::opgs16::manager::SceneManager */
#include "../../../../Shader/shader_wrapper.h"
#include "../../../../Components/Public/preprocessing_getter.h"
#include "../../../../Components/Public/preprocessing2d_renderer.h"

namespace opgs16::builtin::sample {

__S_BACKGROUND::__S_BACKGROUND() {
    SetScaleValue(128.f);
    SetWorldPosition({ 128, 112, 0 });

    component::CPreProcessingGetter* getter = AddComponent<component::CPreProcessingGetter>(*this, "Background");
    m_renderer = AddComponent<component::CPreProcessing2DRenderer>(*this, getter, "gQuad");
    
    /*! Wrapper value setting */
    {
        auto& wrapper = m_renderer->GetWrapper();
        wrapper.SetUniformValue("uTexelLD", glm::vec2{ 0.f, 0.f });
        wrapper.SetUniformValue("uTexelRU", glm::vec2{ 1.f, 1.f });
        wrapper.SetUniformValue<float>("alpha", 1.0f);
    }
}

void __S_BACKGROUND::Render() {
    if (m_renderer) {
        using manager::MSceneManager;
        const auto pvm = MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix();
        m_renderer->GetWrapper().SetUniformValue<glm::mat4>("projection", pvm);
        m_renderer->Render();
    }
}

} /*! ::opgs16::builtin::sample */