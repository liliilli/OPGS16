#ifndef OPGS16_HELPER_VECTOR_H
#define OPGS16_HELPER_VECTOR_H

///
/// @file
///
/// @brief
///
/// @author
///

#include <LinearMath/btVector3.h>

namespace opgs16 {

///
/// @struct DVector2
///
/// @brief
///
struct DVector2 {
  float x, y;

  operator btVector3() const noexcept {
    return btVector3{x, y, 0.f};
  }
};

///
/// @struct DVector3
///
/// @brief
/// Helper vector class.
///
struct DVector3 {
  float x, y, z;

  operator btVector3() const noexcept {
    return btVector3(x, y, z);
  }
};

}

#endif /// OPGS16_HELPER_VECTOR_H