#ifndef OPGS16_HELPER_PHYSICS_ENUM_TYPES_H
#define OPGS16_HELPER_PHYSICS_ENUM_TYPES_H

///
/// @file Helper/Physics/enum_types.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-06 Create file.
///

namespace opgs16 {

///
/// @enum EColliderType
///
/// @brief
/// Supporting collision shape types.
///
enum class EColliderType {
  Box2D,
  Box3D,
  Sphere,
  Convex2D,
  None      // Assertion!
};

///
/// @enum EColliderReturnType
///
/// @brief
///
///
enum class EColliderReturnType {
  Failed,
  Exist,
  Generated
};

///
/// @enum EColliderRemoveResult
///
/// @brief
///
///
enum class EColliderRemoveResult {
  Success,
  NotFound,
  Failed
};

///
/// @enum EColliderFindResult
///
/// @brief
///
///
enum class EColliderFindResult : bool {
  NotFound = false,
  Found = true
};

///
/// @enum ECollisionShapeUsed
///
/// @brief
///
enum class ECollisionShapeUsed : bool {
  NotUsed = false,
  Used = true
};

///
/// @num EShapeReleaseSign
///
enum class EShapeReleaseSign : bool {
  Wait = false,
  Release = true,
};

///
/// @enum ERigidbodyBound
///
/// @brief
///
///
enum class ERigidbodyBound : bool {
  NotBind = false,
  Binded = true,
};

///
/// @enum EDynamic
///
/// @brief
/// 各オブジェクトの動的運動か静的運動かを決めるフラグタイプ。
///
enum class EDynamic {
  Dynamic,
  Kinetic,
  Static
};

}

#endif /// OPGS16_HELPER_PHYSICS_ENUM_TYPES_H
