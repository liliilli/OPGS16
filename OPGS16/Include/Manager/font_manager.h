#ifndef OPGS16_SYSTEM_FONT_MANAGER_H
#define OPGS16_SYSTEM_FONT_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/font_manager.h
///
/// @brief
/// Fundamental font renderer to render string, and manages font glyphs.
/// This file consists of application operation class and member API functions.
///
/// @author Jongmin Yun
/// @log
/// 2018-02-06 Create file.
/// 2018-03-03 Refactoring.
/// 2018-05-26 Remove singleton and replace it with namespace at first.
///

#include <string>
#include <memory>
#include <map>
#include <optional>

/// ::opgs16::manager::_internal::Character
#include <Manager\Internal\font_internal.h>

///
/// @namespace opgs16::manager::font
/// @brief The class manages reading fonts, rendering fonts.
///
/// This class manages reading fonts, rendering fonts.
/// default shader is initiated as creating font instance.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-03 Refactoring. Move it to opgs16::manager.
/// 2018-05-28 Refactoring. Detach rendering part to component::CFont2DRenderer.
///
namespace opgs16::manager::font {

///
/// Internal type aliasing
///

using font_type	    = std::map<char16_t, _internal::Character>;
using font_map_ptr  = std::unique_ptr<font_type>;
using font_raw      = font_type*;

///
/// @brief
/// Initialize Font renderer instance with font path.
///
void Initiate();

///
/// @brief Generate font glyphs with name_tag from ResourceManager.
/// @param[in] name_tag Name tag to refer to stored font glyphs later,
/// and get information from ResourceManager.
///
/// @return The success flag.
///
bool GenerateFont(const std::string& name_tag);

///
/// @brief Loads default font. If default is not specified, return false flag.
/// @return The success flag.
///
font_type* GetDefaultFont();

///
/// @brief
///
///
/// @param[in] font_name_tag
///
/// @return
///
std::optional<font_type*> GetFontSetPtr(const std::string& font_name_tag);

/**
 * @brief Delete font that is specified by tag name. if not found, return false flag.
 * @param[in] tag The tag destroy stored font glyphs.
 * @reutrn The success flag.
 */
bool DeleteFont(const std::string& tag);

///
/// @brief Check font exists.
/// @param[in] font_tag Font name to find in font container.
/// @return The success flag. If font is exist, return true.
///
bool IsFontExist(const std::string& font_tag);

///
/// @brief
/// Get default font size.
///
unsigned GetDefaultFontSize();

} /// ::opgs16::manager::font

#endif /// OPGS16_SYSTEM_FONT_MANAGER_H
