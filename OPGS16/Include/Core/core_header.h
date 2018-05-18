#ifndef OPGS16_SYSTEM_CORE_CORE_HEADER_H
#define OPGS16_SYSTEM_CORE_CORE_HEADER_H

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
 * @file Core/core_header.h
 * @brief Header file list to include on application.cc
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-01 Create file.
 * 2018-03-03 Refactoring.
 */

#include <Element\Canvas\canvas.h>
#include <Manager\font_manager.h>
#include <Manager\input_manager.h>
#include <Manager\object_manager.h>
#include <Manager\physics_manager.h>
#include <Manager\postprocessing_manager.h>
#include <Manager\resource_manager.h>
#include <Manager\scene_manager.h>
#include <Manager\setting_manager.h>
#include <Manager\shader_manager.h>
#include <Manager\sound_manager.h>
#include <Manager\time_manager.h>
#include <Manager\timer_manager.h>

#include "../../Objects/Debug/debug_canvas.h"     /*! CanvasDebug */
#include "../../System/Shader/PostProcessing/pp_convex.h"
#include "../../System/Shader/PostProcessing/pp_sinewave.h"
#include "../../System/Shader/PostProcessing/pp_gray.h"

#endif