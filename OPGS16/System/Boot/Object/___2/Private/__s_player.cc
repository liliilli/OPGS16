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
 * @file System/Boot/Object/___2/Private/__s_player.cc
 * @brief Definition file of ../Public/__s_player.h
 * @log
 * 2018-04-15 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_player.h"
#include "../../../../Components/Public/camera.h"           /*! ::opgs16::component::Camera */
#include "../../../../Components/Public/sprite_renderer.h"  /*! ::opgs16::component::CSprite2DRenderer */
#include "../../../../Manager/Public/scene_manager.h"       /*! ::opgs16::manager::SceneManager */
#include "../../../../Shader/shader_wrapper.h"
#include "../../../Scripts/___2/Public/__s_script_player.h" /*! ::opgs16::builtin::sample::__S_SCRIPT_PLAYER */

namespace opgs16 {
namespace builtin {
namespace sample {

__S_PLAYER::__S_PLAYER() {
    SetScaleValue(12.f);
    SetLocalPosition({ 0, -80, 0 });
    SetRotationLocalAngle(-60.f);
    SetRotationLocalFactor({ 1, 0, 0 });

    //SetRotationFromParentAngle(45.f);
    //SetRotationFromParentFactor({ 0, 0, 1 });

    renderer = AddComponent<component::CSprite2DRenderer>(*this, "System", "gQuad");
    renderer->SetTextureIndex(14);
    AddComponent<__S_SCRIPT_PLAYER>(*this);
}

void __S_PLAYER::Render() {
    using manager::MSceneManager;
    using component::CSprite2DRenderer;

    if (renderer) {
        const auto pvm = MSceneManager::Instance().PresentScene()->GetMainCamera()->PvMatrix() * GetModelMatrix();
        renderer->Wrapper().SetUniformValue<glm::mat4>("projection", pvm);
        GetComponent<CSprite2DRenderer>()->RenderSprite();
    }
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */