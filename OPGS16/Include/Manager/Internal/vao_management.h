#ifndef OPGS16_MANAGER_INTERNAL_VERTEX_ARRAY_MANAGEMENT_H
#define OPGS16_MANAGER_INTERNAL_VERTEX_ARRAY_MANAGEMENT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/vao_management.h
///
/// @brief
/// Internal vertex array object management file could not access from users
/// directly.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-09 Create file.
/// 2018-06-13 Add function.
///

#include <Phitos/Enums/found.h>
#include <Phitos/Enums/success.h>

/// ::opgs16::element::CVaoContainer
#include <Element/Internal/vao_container.h>

namespace opgs16::manager::_internal::vao {

///
/// @brief
/// Initiate vao management.
/// This function must be called only once.
///
void Initiate();

///
/// @brief
/// Shutdown and release resource of vao.
/// This function must be called only once.
///
void Shutdown();

///
/// @brief
/// Get vao resource with success flag.
///
/// @param[in] vao_name Internal VAO resource instance name you find.
///
/// @return If find, get a pair which left side is weak_ptr of Internal VAO
/// instance which has same name you input and right side is EFound::Found.
/// But otherwise unknown ptr and EFound::NotFound.
///
std::pair<element::CVaoContainer*, phitos::enums::EFound>
FindVaoResource(const std::string& vao_name);

///
/// @brief Generate CVaoContainer resource model with model_name from
/// model information container, generate and return with success flag.
///
/// @param[in] model_name Model name to generate vao object.
///
/// @return If successful, return CVaoContainer pointer and flag.
///
std::pair<element::CVaoContainer*, phitos::enums::ESucceed>
GenerateVaoResourceWithModel(const std::string& model_name);

///
/// @brief Just find vao object is exist.
/// @param[in] vao_name Vao name or mesh name or model name to find bound vao.
/// @return Found flag.
///
phitos::enums::EFound IsVaoResourceExist(const std::string& vao_name);

} /// ::opgs16::manager::_internal::vao namespace.

#endif ///OPGS16_MANAGER_INTERNAL_VERTEX_ARRAY_MANAGEMENT_H