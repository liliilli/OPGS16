#ifndef OPGS16_MANIFEST_USER_CONFIGURE_H
#define OPGS16_MANIFEST_USER_CONFIGURE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

namespace opgs16::manifest {

///
/// IF YOU WANT TO USE "YOUR OWN" PROJECT, UNCOMMENT THIS BELOW.
///
#define _CUSTOM_PROJECT

// If you want to turn on DEBUG MODE to display console log and special debug display,
// Remove "//" Commend specifier.
#define _OPGS16_DEBUG_OPTION

///
/// PROJECT SETTING
///

// INITIAL SCREEN SIZE
constexpr unsigned k_size{ 2 };

// To use your own application setting, please uncomment #define _CUSTOM_PROJECT.
#if defined(_CUSTOM_PROJECT)

// Set Application project name.
#define _APPLICATION_PROJECT_NAME \
L"TEST GAME"

// Set Application window name when operating. If this is not uncommented, just use PROJECT_NAME insteadily.
//#define _APPLICATION_WINDOW_NAME

// set resource setting file to load your resource on run-time.
// This must be written as the style of " ... " or application will not build up.
#define _RESOURCE_SETTING_FILE_PATH \
"_Project/Debug/ProjectSetting/resource.meta"

//
#define _INITIAL_SCENE_INCLUDE_PATH \
<../_Project/Debug/Include/Scene/object_lifecycle.h>

// set first scene of your project.
// This must be written as the style of L" ... " or application will not build up.
#define _INITIAL_SCENE_FULL_NAME \
::debug::scene::ObjectLifeCycle

///
/// Decide whether show the boot screen logo when application is executed.
/// If you want to show the logo, set this true or false.
///
#define _SHOW_BOOT_SCREEN \
false

#endif

} /// opgs16::manifest

#endif /// OPGS16_MANIFEST_USER_CONFIGURE_H