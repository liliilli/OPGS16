
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
///

/// Header file
#include <Manager/mesh_manager.h>

#include <unordered_map>

#include <Phitos/Dbg/assert.h>
#include <Phitos/Enums/initiated.h>

/// ::opgs16::element::DModelObject
#include <Element/Internal/model_object.h>
#include "Element/Default/model_2dquad.h"

//!
//! Data and flags
//!

namespace {
using phitos::enums::EInitiated;
EInitiated b_initiated = EInitiated::NotInitiated;
EInitiated b_builtin_initiated = EInitiated::NotInitiated;

using ModelMap = std::unordered_map<std::string, opgs16::element::DModelObject>;
ModelMap m_model_map = {};

} /// unnamed namespace

//!
//! Global static functions.
//!

namespace {

///
/// @brief
///
///
void InitiateBuiltinModelMeshes() {
  PHITOS_ASSERT(b_initiated == EInitiated::NotInitiated,
      "Built-in mesh initiation must be called in Initiate() funciton.");
  PHITOS_ASSERT(b_builtin_initiated == EInitiated::NotInitiated,
      "Duplicated built-in model initiation must be called only once.");

  using opgs16::builtin::model::BModel2DQuad;
  m_model_map.try_emplace(BModel2DQuad::m_model_name.data(), BModel2DQuad{});

  b_builtin_initiated = EInitiated::Initiated;
}

}

//!
//! Definitions
//!

namespace opgs16::manager::mesh {

void Initiate() {
  PHITOS_ASSERT(b_initiated == EInitiated::NotInitiated,
      "Duplicated mesh manager initiation is prohibited.");

  InitiateBuiltinModelMeshes();

  // Do not change the order.
  b_initiated = EInitiated::Initiated;
}

void Shutdown() {
  PHITOS_ASSERT(b_initiated == EInitiated::NotInitiated,
      "Could not release not initiated mesh manager.");
  b_initiated = EInitiated::NotInitiated;
}

} /// ::opgs16::manager::mesh namespace