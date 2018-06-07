#ifndef OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H
#define OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/shader_manager.h
///
/// @brief
/// Shader management class.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-04 Refactoring.
/// 2018-03-11 Correction of object namespace position and path.
/// 2018-06-07 Detach class structure to namespace.
///

#include <memory>
#include <unordered_map>

/// ::opgs16::manager::resource_manager
#include <Manager/resource_manager.h>
/// ::opgs16::element::CObject
#include <Element/object.h>
/// ::opgs16::element::CShaderNew
#include <Shader/shader.h>

///
/// @namespace opgs16::manager::shader
/// @brief
/// manages shaders on runtime in scene.
///
/// ShaderManager class manages shaders compiled on runtime in scene, These are able to bind
/// the object in use, render binded m_object_list, move binded object to another shader, and be deleted.
///
/// Shaders in container are managed by shader name, and Tags.
/// ShaderManager is singleton, so user cannot create another ShaderManager instance.
///
/// @author Jongmin Yun
/// @log
/// 2018-02-06 Create file and make basic features.
/// 2018-03-04 Refactoring.
/// 2018-06-07 Detach class structure to namespace.
///
///@todo Implement ReleaseAll(), ReleaseShader(), CheckError();
///
namespace opgs16::manager::shader {

using shader_raw = element::CShaderNew*;
using shader_ptr = std::unique_ptr<element::CShaderNew>;
using shader_map = std::unordered_map<std::string, shader_ptr>;

///
/// @brief
/// Initiate shader manager.
///
void Initiate();

///
/// @brief
/// Release all shader programs and information.
///
void ReleaseAll();

///
/// @brief
///
///
/// @param[in] shader_name The name tag to release.
///
void ReleaseShader(const std::string& shader_name);

///
/// @brief The method returns arbitary shader which has tag name.
/// If the method fails to search shader has tag name, create shader and return.
///
/// @param[in] name The tag to find.
/// @return Founded shader pointer, if fails create shader and return late.
///
inline shader_raw GetShader(const std::string& name);

} /// ::opgs16::manager namespace

#endif /// OPGS16_SYSTEM_SHADER_SHADER_MANAGER_H