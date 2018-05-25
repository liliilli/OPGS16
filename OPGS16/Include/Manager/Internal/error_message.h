#ifndef OPGS16_MANAGER_INTERNAL_ERROR_MESSAGE_H
#define OPGS16_MANAGER_INTERNAL_ERROR_MESSAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/error_message.h
///
/// @brief
/// Manages global settings such as tag, rendering layer, collision layer
/// for application.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-20 Create file and pile up error messages.
///

namespace opgs16::debug {

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// ::opgs16::manager::setting error messages.
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

constexpr const char err_setting_not_initiated[] =
    "Setting is not initiated yet.";
constexpr const char err_setting_duplicated_init[] =
    "Duplicated function call of "
    "::opgs16::manager::setting::Initiate() is prohibited.";

constexpr const char err_setting_tag_oob[] =
    "Tag index is out of bound.";
constexpr const char err_setting_rendering_oob[] =
    "Rendering layer index is out of bound.";
constexpr const char err_setting_collision_oob[] =
    "Collision layer index is out of bound.";
constexpr const char err_setting_collision_check_oob[] =
    "Collision check index is out of bound.";

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// ::opgs16::manager::input error messages.
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

constexpr const char err_input_not_initiated[] =
    "Input Setting its not initiated yet.";
constexpr const char err_input_duplicated_init[] =
    "Duplicated function call of "
    "::opgs16::manager::input::Initiate(context) is prohibited.";

/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
/// ::opgs16::manager::object error messages.
/// ---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*

constexpr const char err_object_duplicated_init[] =
    "Duplicated function call of "
    "::opgs16::manager::object::Initaite() is prohibited.";

constexpr const char err_object_out_of_bound_rendering_list[] =
    "Rendering list index is out of bound";

} /// ::opgs16::manager::_internal

#endif /// OPGS16_MANAGER_INTERNAL_ERROR_MESSAGE_H
