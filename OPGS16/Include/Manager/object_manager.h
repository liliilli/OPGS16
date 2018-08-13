#ifndef OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/object_manager.h
///
/// @brief
/// This file have management namespace which is related to management of object.
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Refactoring.
/// 2018-03-05 Add Render function and rendering layer container.
/// 2018-05-25 Recode singleton class to namespace structure.
///

/// Forward declaration
#include <opgs16fwd.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CParticleEmitter;
class CParticleSpawner;
}

namespace opgs16::component::_internal {
  class CPrivateXyzAxisRenderer;
  class CPrivateAabbRendererBase;
}

//!
//! Implementation
//!

///
/// @namespace opgs16::manager::object
/// @brief
/// Namespace for managing objects in scene.
/// This namespace has object list, objects in list will be update, render, and
/// be destroyed by calling provided function.
///
namespace opgs16::manager::object {

///
/// @brief
/// Let load and initiate initial setting to be used in application.
/// This function must be called only once out of entire application.
///
void Initiate();

///
/// @brief
/// Destroy candidate objects in destroy list.
///
void Update(float delta_time);

///
/// @brief
/// Render all specified object which can be rendered on display or framebuffer.
/// After rendering, all item reference addresses in list are cleared.
///
void Render();

///
/// @brief
/// Render all specified Axis-aligned bounding box information with line.
///
/// @todo Write comments
///
void RenderAABB();

///
/// @brief
/// Destroy specified object in present scene.
///
/// If the object has script component, object will call DestroyGameObject() function
/// before deleted permanently.
///
/// @param[in] object Object to be destroyed.
/// @param[in] root Object start point.
///
bool DestroyGameObject(const element::CObject& object,
                       element::CObject* root = nullptr,
                       bool is_recursive = false);

///
/// @brief
/// Clear all destroy candidates.
///
void ClearDestroyCandidates();

///
/// @brief
/// Clear all rendering candidates.
///
void ClearRenderingList();

///
/// @brief
/// Helper function for deleting all list in candidate lists.
///
inline void ClearAll() {
  ClearDestroyCandidates();
  ClearRenderingList();
}

///
/// @brief
/// Bind object reference which will be rendered with rendering layer index.
/// Bound obejct reference's Render() function will be called by object::Render()
///
/// If layer index is bigger than permitted layer index size,
/// In debug mode this asserts and halt program,
/// but undefined behaviour in release mode.
///
/// @param[in] object GameObject reference pointer to be rendered.
/// @param[in] layer_index Rendering layer index.
///
void InsertRenderingObject(element::CObject* const object, unsigned layer_index);

///
/// @brief
///
/// @param[in] aabb_component
///
void InsertAABBInformation(component::_internal::CPrivateAabbRendererBase& aabb_component);

///
/// @brief
///
/// @param[in] emitter_component
///
void InsertParticleEmitter(component::CParticleEmitter& emitter_component);

///
/// @brief
///
void pBindRenderXyzAxisRenderer(component::_internal::CPrivateXyzAxisRenderer& component);

///
/// @brief
///
void pMoveParticleSpawner(std::unique_ptr<component::CParticleSpawner>& particle_spawner);

} /// ::opgs16::manager::object

#endif /// OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
