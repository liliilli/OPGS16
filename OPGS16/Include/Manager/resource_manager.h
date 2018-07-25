#ifndef OPGS16_MANAGER_RESOURCE_H
#define OPGS16_MANAGER_RESOURCE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager\resource.h
///
/// @brief
/// Manages resource meta information and loading path.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-27 Correction of loading mechanism
/// 2018-02-28
/// Corrected GetTexture2D return type must be resource::STexture2D structure.
/// 2018-03-02
/// Move ResourceManager class inside opgs16::manager for uniformation.
/// 2018-03-03
/// Refactored and removed member functions are not appropriate for class.
/// 2018-04-06
/// Support Texture2DAtlas instead of plain Texture2D
/// which not support atlas information.
/// 2018-05-22
/// Remove class singleton and renovate it to namespace for uniformation.
///

#include <string>
#include <map>

#include <Manager\resource_type.h>
#include <opgs16fwd.h>      /// Forward declaration

///
/// @namespace opgs16::manager::resource
/// @brief This class is singleton and not derivable as derived class.
/// resource namespace has a rule of managing resource files path,
/// each shader path, error statement path and so on.
///
/// @log
/// 2018-02-27 Correction of loading mechanism
/// 2018-02-28
/// Corrected GetTexture2D return type must be resource::STexture2D structure.
/// 2018-03-02 Move to opgs16::manager namespace.
/// 2018-03-03
/// Move private functions which don't match class to non-member function.
/// 2018-04-06
/// Support Texture2DAtlas instead of plain Texture2D
/// which not support atlas information.
/// 2018-05-22
/// Remove class singleton and renovate it to namespace for uniformation.
///
namespace opgs16::manager::resource {

///
/// @brief
/// Get specific shader list.
///
/// @param[in] name_key shader program aliasing name.
/// @return Specific shader container.
///
const opgs16::resource::SShader* GetShader(const std::string& name_key);

///
/// @brief
/// Get specific texture path.
///
/// @param[in] name_key Wrapping tag name of actual sprite path.
/// @return Selected 2D texture path.
///
const opgs16::resource::STexture2DAtlas* GetTexture2D(const std::string& name_key);

///
/// @brief
/// Get specified sound path information.
///
/// @param[in] name_key Wrapping tag name of actual sound path.
/// @return Sound path
///
const opgs16::resource::SSoundInfo* GetSound(const std::string& name_key);

///
/// @brief
/// Return font information with success flag.
///
/// @param[in] name_key Name tag of font information you want to find
/// @return success flag, and information
///
const opgs16::resource::SFont* GetFont(const std::string& name_key);

///
/// @brief
/// Return animation information pointer. if doesn't find return nullptr.
///
/// @param[in] name_key Name tag of animation resource information you want to find
/// @return The pointer of animation information otherwise nullptr.
///
const opgs16::resource::SAnimation* GetAnimation(const std::string& name_key);

/// Internal namespace
namespace __ {

///
/// @brief
/// Initiate resource management and read appropriate files.
/// builtin.meta
/// resource.meta
///
void Initiate();

///
/// @brief
/// Clean up and release all heap resources.
///
void Shutdown();

} /// ::opgs16::manager::resource::__ namespace

} /// ::opgs16::manager::resource

#endif /// OPGS16_MANAGER_RESOURCE_H */