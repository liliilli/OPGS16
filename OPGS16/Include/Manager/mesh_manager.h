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

} /// ::opgs16::manager::mesh namespace

#endif /// OPGS16_MANAGER_INTERNAL_MESH_MANAGER_H