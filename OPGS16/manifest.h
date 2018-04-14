#ifndef OPGS16_MANIFEST_USER_CONFIGURE_H
#define OPGS16_MANIFEST_USER_CONFIGURE_H

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

namespace opgs16 {
namespace manifest {

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * IF YOU USE "YOUR OWN" PROJECT, UNCOMMENT THIS BELOW.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/
//#define _CUSTOM_PROJECT

// If you want to turn on DEBUG MODE to display console log and special debug display,
// Remove "//" Commend specifier.
#define _OPGS16_DEBUG_OPTION

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * PROJECT SETTING
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

// INITIAL SCREEN SIZE
constexpr unsigned k_size{ 2 };

// To use your own application setting, please uncomment #define _CUSTOM_PROJECT.
#if defined(_CUSTOM_PROJECT)

// Set Application project name.
#define _APPLICATION_PROJECT_NAME   L"OPGS16 SAMPLE GAME"
// Set Application window name when operating. If this is not uncommented, just use PROJECT_NAME insteadily.
//#define _APPLICATION_WINDOW_NAME

// set resource setting file to load your resource on run-time.
// This must be written as the style of L" ... " or application will not build up.
#define _RESOURCE_SETTING_FILE_PATH L"_Project/Maintenance/_meta/_resource.meta"

//
#define _INITIAL_SCENE_INCLUDE_PATH "../../../_Project/Maintenance/Scene/test_1.h"
// set first scene of your project.
// This must be written as the style of L" ... " or application will not build up.
#define _INITIAL_SCENE_FULL_NAME    Maintenance

// true or false
#define _SHOW_BOOT_SCREEN           true

#endif

} /*! opgs16::manifest */
} /*! opgs16 */

#endif // OPGS16_MANIFEST_USER_CONFIGURE_H