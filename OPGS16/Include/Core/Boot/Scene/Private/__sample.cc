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
 * @file System/Boot/Scene/Private/__sample.cc
 * @brief Sample game scene.
 * @author Jongmin Yun
 * @log
 * 2018-04-07 Create file.
 * 2018-04-15 Add Base objects.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__sample.h" /*! Header file */

#include "../../Object/___2/Public/__s_camera.h"    /*! ::ogps16::builtin::sample::__S_PERSPECTIVE_CAMERA */
#include "../../Object/___2/Public/__s_manager.h"   /*! ::opgs16::builtin::sample::__S_MANAGER */
#include "../../Object/___2/Public/__s_background.h"/*! ::opgs16::builtin::sample::__S_BACKGROUND */

#include "../../../Manager/Public/prerendering_manager.h"
#include "../../Object/___2/Effects/__s_starfield.h"
#include "../../Object/___2/Effects/__s_turnel.h"

constexpr const char* k_main_camera = "Camera";
constexpr const char* k_cursor_name = "Cursor";
constexpr const char* k_canvas_name = "Canvas";
constexpr const char* k_manager_name = "Manager";
constexpr const char* k_background_pre = "Background";

namespace opgs16 {
namespace builtin {
namespace sample {

void SampleGame::Initiate() {
    auto bg_color = BackgroundColor();
    //bg_color->r = 0.2f;

    /*! Make background pre-rendering container. */
    auto pre_back = manager::prerendering::GenerateFrameBuffer(k_background_pre);
    if (pre_back) {
        pre_back->InsertFrameBuferFrame<__S_STARFIELD_FRAMEBUFFER>();
        pre_back->InsertFrameBuferFrame<__S_TURNEL_FRAMEBUFFER>();
        pre_back->Initialize();
    }
    else {
        assert(false);
    }

    Instantiate<__S_PERSPECTIVE_CAMERA>(k_main_camera);
    Instantiate<__S_MANAGER>(k_manager_name);
    Instantiate<__S_BACKGROUND>(k_background_pre);
}

} /*! opgs16::builtin::sample */
} /*! opgs16::builtin */
} /*! opgs16 */
