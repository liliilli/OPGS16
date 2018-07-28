#ifndef OPGS16_MANAGER_INTERNAL_PHYSICS_FLAG_H
#define OPGS16_MANAGER_INTERNAL_PHYSICS_FLAG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/physics_flag.h
///
/// @brief
/// Manages enum flags for physics manager.
///
/// @log
/// 2018-06-01 Create file
///

namespace opgs16::manager::physics {

///
/// @enum ECollided
///
/// @brief
/// String enum boolean flag for checking collision box collision.
///
enum class ECollided : bool {
  NotCollided = false,
  Collided = true
};

} /// ::opgs16::manager::physics namespace

#endif /// OPGS16_MANAGER_INTERNAL_PHYSICS_FLAG_H