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

///
/// Set Application project name.
///

#define OP16_SETTING_APPLICATION_NAME \
"TestProject"

///
/// Set Project developement path.
///

#define OP16_SETTING_APPLICATION_DEVPATH \
"D:/Development/OPGS16/OPGS16/_Project/Test020"

///
/// Set Application window name when operating.
/// If this is not uncommented, just use PROJECT_NAME insteadily.
///

#define OP16_SETTING_APPLICATION_WINDOW_NAME \
"OPGS16 0.2.5 2018-08-14"

///
/// Set Project first scene (entry scene) path from this manifest file.
///

#define OP16_SETTING_FIRST_SCENE_INCLUDE_RELATIVE_PATH \
<../_Project/Test020/Include/Scene/scene_bullet_hello.h>

///
/// set first scene full name of your project.
///

#define OP16_SETTING_FIRST_SCENE_FULL_NAME \
::test020::scene::SceneBulletHello

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
/// INITIAL SCREEN SIZE
///

//#define OP16_SETTING_START_SCALE_X1
#define OP16_SETTING_START_SCALE_X2
//#define OP16_SETTING_START_SCALE_X3

///
/// HARD OPTION : Fixed function fixed fps controller
///
/// You must turn on only one FPS value option
/// even though you are using no-vsync option on application.
///
/// OP16_SETTING_USING_60FPS_ON_VSYNC
/// OP16_SETTING_USING_30FPS_ON_VSYNC
///

#define OP16_SETTING_USING_60FPS_ON_VSYNC
//#define OP16_SETTING_USING_30FPS_ON_VSYNC

///
/// Decide whether show the boot screen logo when application is executed.
/// If you want to show the logo or not, commment #define below.
///

//#define OP16_SETTING_BOOTSCREEN_SHOW

/// If you want to turn on DEBUG MODE to display console log and special debug display,
/// Remove "//" Commend specifier.

#define OP16_SETTING_DEBUG_MODE

///
/// Enable logging uncommenting this.
/// If you are in OP16_SETTING_ENABLE_DEBUG, logging feature will be activated automatically.
///

#define OP16_SETTING_ENABLE_LOGGING

///
/// Enable this to be enable logging to console.
/// To disable this, just wrap macro with comment //.
///

#define OP16_SETTING_ENABLE_LOGGING_TO_CONSOLE

///
/// Enable this to be enable logging to file.
/// To disable this, just wrap macro with commment //.
///

#define OP16_SETTING_ENABLE_LOGGING_TO_FILE

///
/// If you enabled OP16_SETTING_ENABLE_LOGGING_TO_FILE,
/// recommend specify logging file path which file created to.
/// or not, OPGS16 make just "log.txt" file to your root folder.
///

#define OP16_SETTING_LOGGING_FILE_PATH \
".log/opgs16_log.txt"

} /// opgs16::manifest

#endif /// OPGS16_MANIFEST_USER_CONFIGURE_H