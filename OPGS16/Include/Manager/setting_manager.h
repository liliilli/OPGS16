#ifndef OPGS16_MANAGER_SETTING_MANAGER_H
#define OPGS16_MANAGER_SETTING_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/setting_manager.h
///
/// @brief
/// Manages global settings such as tag, rendering layer, collision layer
/// for application.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-26 Removed redundant move special functions.
/// 2018-03-04 Refactoring.
/// 2018-05-20 Detach singleton pattern into namespace.
///

#include <string>
#include <vector>

///
/// @namespace opgs16::manager::setting
///
/// @brief
/// Manages global application settings for layer, rendering layer, tag, etc.
///
/// @log
/// 2018-02-26 Removed redundant move special functions.
/// 2018-03-04 Refactoring.
/// 2018-05-20 Detach singleton pattern into namespace.
///
namespace opgs16::manager::setting {

using TagList = std::vector<std::string>;
using RenderingLayerList = std::vector<std::string>;
using CollisionLayerLIst = std::vector<std::string>;

///
/// @brief
/// Let load and initiate global setting to be used in application.
/// This function must be called at first of entry function.
///
void Initiate();

///
/// @brief
/// Return tag name of specified index.
/// If input id exceeds the size of tag list, return "ERROR::TAG::OOB"
///
/// @param[in] i index number to find in tag list.
/// @return Tag name of index.
///
const std::string& GetTagName(uint32_t i);

///
/// @brief
/// Return rendering layer name of specified index.
/// If input value exceeds the size of rendering layer list,
/// occurs assert in debug mode and immediate UB in release mode.
///
/// @param[in] i index number to find in rendering layer list.
/// @return Rendering layer name of index.
///
const std::string& GetRenderingLayerName(uint32_t i);

///
/// @brief
/// Return tag name list.
/// This function call takes a lot of time, you have been recommended to use
/// it only in debug mode.
///
/// @return Tag name list.
///
const TagList& GetTagNameList();

///
/// @brief
/// Return rendering layer name list.
/// This function call takes a lot of time, you have been recommended to use
/// it only in debug mode.
///
/// @return Rendering layer name list.
///
const RenderingLayerList& GetRenderingLayerNameList();

///
/// @brief
/// Return rendering layer name list size.
///
/// @return The size of rendering layer name list.
///
uint32_t GetRenderingLayerNameListSize();

///
/// @brief
///
/// @param[in] i index number to find in collision layer list.
/// @return Collision layer name of index.
///
const std::string& GetCollisionLayerName(uint32_t i);

/// @brief
/// Return collision layer list.
/// This function call takes a lot of time, you have been recommended to use
/// it only in debug mode.
///
/// @return Rendering layer name list.
///
const CollisionLayerLIst& GetCollisionLayerList();

///
/// @brief
/// Get collision check flag of collision layers.
///
/// @return bool
///
/// @todo Replace boolean return type to explicit enum status type.
///
bool CollisionLayerCheck(unsigned lhs, unsigned rhs);

} /// ::opgs16::manager::setting

#endif /// OPGS16_SYSTEM_MANAGER_SETTING_MANAGER_H
