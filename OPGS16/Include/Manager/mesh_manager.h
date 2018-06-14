#ifndef OPGS16_MANAGER_INTERNAL_MESH_MANAGER_H
#define OPGS16_MANAGER_INTERNAL_MESH_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/mesh_manager.h
///
/// @brief
/// Internal management file of meshes. 2d, 3d.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-10 Create file.
/// 2018-06-12 Add feature of models.
///

#include <string>

/// ::phitos::enums::EFound boolean strong enum type
#include <Phitos/Enums/found.h>
#include <Phitos/Enums/success.h>

/// ::opgs16::element::_internal::CInternalVertexArrayObject
#include <Element/Internal/internal_vertex_array_object.h>
/// ::opgs16::element::CVaoContainer.
#include <Element/Internal/vao_container.h>

namespace opgs16::manager::mesh {

///
/// @brief
///
///
void Initiate();

///
/// @brief
///
///
void Shutdown();

///
/// @brief
/// Check if model information object is exist on memory.
///
/// @param[in] model_name Model name to find, model_name must be same
/// on resource model name to have been specified.
///
/// @return If found return EFound::Found but EFound::NotFound.
///
phitos::enums::EFound IsModelExist(const std::string& model_name);

///
/// @brief
///
///
/// @param[in] resource_model_name
///
/// @return
///
phitos::enums::ESucceed GenerateModel(const std::string& resource_model_name);

///
/// @brief
///
///
/// @param[in] model_name
///
/// @return
///
///
std::pair<std::unique_ptr<element::CVaoContainer>, phitos::enums::ESucceed>
GenerateVaoItemsFromModel(const std::string& model_name);

///
/// @brief
///
/// @param[in] model_name
/// @param[in] vbo_type
/// @param[in] ebo_type
///
/// @return
///
std::pair<std::unique_ptr<element::CVaoContainer>, phitos::enums::ESucceed>
GenerateVaoItemsFromModelExt(
    const std::string& model_name,
    element::_internal::EVboBufferType vbo_type,
    element::_internal::EEboBufferType ebo_type);

} /// ::opgs16::manager::mesh namespace

#endif /// OPGS16_MANAGER_INTERNAL_MESH_MANAGER_H