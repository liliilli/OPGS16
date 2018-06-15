#ifndef OPGS16_HELPER_VECTOR_H
#define OPGS16_HELPER_VECTOR_H

///
/// @file
///
/// @brief
///
/// @author
///

#include <array>

#include <assimp/vector3.h>
#include <LinearMath/btVector3.h>

namespace opgs16 {

///
/// @struct DVector2
///
/// @brief
///
struct DVector2 {
  float x = 0.f;
  float y = 0.f;

  DVector2() noexcept {};

  DVector2(float x, float y) :
      x{x}, y{y} {};

  operator btVector3() const noexcept {
    return btVector3{x, y, 0.f};
  }

  DVector2(const aiVector3D& value) noexcept :
    x{value.x}, y{value.y} {};

  DVector2& operator=(const aiVector3D& value) noexcept {
    x = value.x;
    y = value.y;

    return *this;
  }
};

///
/// @struct DVector3
///
/// @brief
/// Helper vector class.
///
struct DVector3 {
  float x = 0.f;
  float y = 0.f;
  float z = 0.f;

  DVector3() noexcept {};

  DVector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {};

  DVector3(const aiVector3D& value) noexcept :
      x{value.x}, y{value.y}, z{value.z} {}

  ///
  /// @brief
  /// Return sequence data of this instance.
  ///
  std::array<float, 3> Data() const noexcept {
    return {x, y, z};
  }

  DVector3& operator=(const aiVector3D& value) noexcept {
    x = value.x;
    y = value.y;
    z = value.z;

    return *this;
  }

  operator btVector3() const noexcept {
    return btVector3(x, y, z);
  }
};

}

#endif /// OPGS16_HELPER_VECTOR_H