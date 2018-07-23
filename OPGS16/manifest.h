#include "precompiled.h"
#ifndef OPGS16_MANIFEST_USER_CONFIGURE_H
#define OPGS16_MANIFEST_USER_CONFIGURE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

namespace opgs16::manifest {

/// If you want to turn on DEBUG MODE to display console log and special debug display,
/// Remove "//" Commend specifier.
//#define _OPGS16_DEBUG_OPTION

/// INITIAL SCREEN SIZE
constexpr unsigned k_size = 1;

/// Set Application project name.
#define _APPLICATION_PROJECT_NAME \
"Enter Your own game name"

/// Set Project path
#define _APPLICATION_PROJECT_PATH \
"D:/Development/OPGS16/OPGS16/_Project/Template"

// Set Application window name when operating.
// If this is not uncommented, just use PROJECT_NAME insteadily.
//#define _APPLICATION_WINDOW_NAME

#define _INITIAL_SCENE_INCLUDE_PATH <../_Project/Template/Include/hello_world.h>

/// set first scene full name of your project.
#define _INITIAL_SCENE_FULL_NAME ::SceneFirstHelloWorld

///
/// Fixed function resolution feature
///
/// OP16_SETTING_RESOLUTION_256224
/// OP16_SETTING_RESOLUTION_320240
/// OP16_SETTING_RESOLUTION_400240
/// OP16_SETTING_RESOLUTION_640480
///

//#define OP16_SETTING_RESOLUTION_256224
//#define OP16_SETTING_RESOLUTION_320240
#define OP16_SETTING_RESOLUTION_400240
//#define OP16_SETTING_RESOLUTION_640480

///
/// Decide whether show the boot screen logo when application is executed.
/// If you want to show the logo, set this true or false.
///
#define _SHOW_BOOT_SCREEN true

} /// opgs16::manifest

#endif /// OPGS16_MANIFEST_USER_CONFIGURE_H