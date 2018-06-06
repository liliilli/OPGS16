#ifndef OPGS16_MANAGER_INTERNAL_PHYSICS_COLLISION_SHAPE_LIST_H
#define OPGS16_MANAGER_INTERNAL_PHYSICS_COLLISION_SHAPE_LIST_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/physcis_collision_shape_list.h
///
/// @brief
/// Implementation file for collision shape list helper class.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-04 Create file.
///

#include <unordered_map>

#include <LinearMath/btHashMap.h>
/// ::opgs16::element::_internal::CCollisionShapeWrapper
#include <Element/Internal/physics_collision_shape_wrapper.h>
/// ::opgs16 Enum types for physics
#include <Helper/Physics/enum_types.h>

//!
//! Implemenation
//!

namespace opgs16::manager::physics::_internal {

///
/// @class CCollisionShapeList
///
/// @brief
/// Manages collision shape list for calculation and collision checking of
/// physics engine more helpfully.
///
/// @author Jongmin Yun
///
class CCollisionShapeList {
public:
  using CollisionShapeMap =
      std::unordered_map<std::string,
                         element::_internal::CCollisionShapeWrapper>;

  //!
  //! Special functions
  //!

  CCollisionShapeList() = default;
  ~CCollisionShapeList() = default;

  CCollisionShapeList(const CCollisionShapeList&) = delete;
  CCollisionShapeList& operator=(const CCollisionShapeList&) = delete;
  CCollisionShapeList(CCollisionShapeList&&) = delete;
  CCollisionShapeList& operator=(CCollisionShapeList&&) = delete;

  //!
  //! Function definition.
  //!

///
/// @brief
/// Find collision shape with shape name.
/// If find, return iterator position of found collision shape with Found flag;
/// Otherwise, return NotFound flag and must not use iterator.
///
/// @param[in] shape_name Collision shape name to find.
///
/// @return If found, accessible iterator position of collision shape and Found
/// flag. If not find, unknown position iterator and NotFound flag.
///
inline std::pair<CollisionShapeMap::iterator, EColliderFindResult>
    FindCollisionShape(const std::string& shape_name) {
  auto it = m_collision_map.find(shape_name);

  EColliderFindResult result_flag = EColliderFindResult::Found;
  if (it == m_collision_map.end())
    result_flag = EColliderFindResult::NotFound;

  return {it, result_flag};
}

///
/// @brief
/// Check collision shape is exist or not.
///
/// @param[in] shape_name Collision shape name to find.
///
/// @return Found or NotFound flag.
///
inline EColliderFindResult IsCollisionExist(const std::string& shape_name) {
  if (m_collision_map.find(shape_name) != m_collision_map.end())
    return EColliderFindResult::Found;
  else
    return EColliderFindResult::NotFound;
}

///
/// @brief
/// Sweep obsolete collision shapes which are used by someone rigidbody and
/// There are no anyone using those now.
///
/// Procedure should be safe and must not remain any fragment of dangling leak
/// memories.
///
inline void SweepObsoleteShapes() {
  for (auto it = m_collision_map.begin(); it != m_collision_map.end();) {
    if (it->second.GetCount() == 0 &&
        it->second.IsUsed() == ECollisionShapeUsed::Used) {
      m_collision_shape_list.remove(it->first.c_str());
      it = m_collision_map.erase(it);
    }
    else {
      ++it;
    }
  }
}

///
/// @brief
/// Try emplace collision shape with shape name as a key, and variadic arguments
/// for generating specific shape class.
/// If there is already collision shape as a key (shape_name), this function will
/// return unknown position iterator with Exist flag.
/// Also, If procedure is failed when emplace function will return unknown posit
///ion iterator with Failed flag.
/// Finally everything is good so return accessible iterator position which indi
/// cates CollisionShape instance and Generated flag.
///
/// @param[in] shape_name Collision shape name as a key name.
/// @param[in] args Arguments to be used construct shape instance.
///
/// @tparam TType The collision shape type to make.
/// @tparam TArgs Variadic arguments type used to make shape instance.
///
/// @return Position iterator indicates generated shape instance and Result type.
///
template <EColliderType TType, typename... TArgs>
std::pair<CollisionShapeMap::iterator, EColliderReturnType>
    TryEmplaceCollisionShape(const char* shape_name, TArgs&&... args) {
  /// Body
  if (auto [it, result] = FindCollisionShape(shape_name);
      result == EColliderFindResult::Found) {
    if (it->second.GetCount() != 0)
      return {it, EColliderReturnType::Exist};

    m_collision_map.erase(shape_name);
    m_collision_shape_list.remove(shape_name);
  }

  auto [it, is_created] = m_collision_map.try_emplace(
      shape_name,
      element::_internal::MakeCollisionShapeWrapper<TType>(
          shape_name,
          std::forward<TArgs>(args)...));

  if (!is_created) {
    m_collision_map.erase(shape_name);
    return {it, EColliderReturnType::Failed};
  }

  const auto& key = it->second.GetShapeName();
  const auto& ptr = it->second.GetShape();
  m_collision_shape_list.insert(key, ptr);

  return {it, EColliderReturnType::Generated};
}

///
/// @brief
/// Remove collision shape which has a name called shape_name.
/// This function must be called after whichever rigidbody and objects use
/// collision shape called shape_name are destroyed, or it will behave undefined
/// behavior or runtime crash.
///
/// This function is not recommendable for calling manually.
/// Instead of calling this directly, consider using SweepObsoleteShapes() or
/// any helper functions.
///
/// @param[in] shape_name Collision shape name to remove.
///
/// @return Result flag.
///
EColliderRemoveResult RemoveCollisionShape(const char* shape_name) {
  if (auto [it, result] = FindCollisionShape(shape_name);
      result == EColliderFindResult::Found) {
    if (const auto ptr = m_collision_shape_list.find(shape_name);
        ptr == nullptr)
      return EColliderRemoveResult::Failed;
    if (it->second.GetCount() > 0)
      return EColliderRemoveResult::Failed;

    m_collision_shape_list.remove(shape_name);
    m_collision_map.erase(it);
    return EColliderRemoveResult::Success;
  }

  return EColliderRemoveResult::NotFound;
}

private:

//
CollisionShapeMap m_collision_map;

//
btHashMap<btHashString, btCollisionShape*> m_collision_shape_list;

};

} /// ::opgs16::manager::physics::_intennal namespace

#endif /// OPGS16_MANAGER_INTERNAL_PHYSICS_COLLISION_SHAPE_LIST_H
