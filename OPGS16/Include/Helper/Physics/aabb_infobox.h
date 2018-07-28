#ifndef OPGS16_HELPER_AABB_INFO_BOX_H
#define OPGS16_HELPER_AABB_INFO_BOX_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/aabb_infobox.h
///

#include <array>

#include <Helper/include_vector.h>

//!
//! Forward declaration.
//!

class btVector3;

//!
//! Implementation.
//!

namespace opgs16 {

enum class EAABBStyle {
  _2D,
  _3D
};

class DAABBInfoBox {
public:
  DAABBInfoBox(const btVector3& min, const btVector3& max);
  DAABBInfoBox(const opgs16::DVector3& min, const DVector3& max);

  ///
  /// @brief
  ///
  ///
  /// @return
  ///
  std::array<DVector2, 4> GetVertexPoints() const noexcept {
    const DVector2 ld = DVector2{m_min.x, m_min.y};
    const DVector2 rd = DVector2{m_max.x, m_min.y};
    const DVector2 ru = DVector2{m_max.x, m_max.y};
    const DVector2 lu = DVector2{m_min.x, m_max.y};
    return {ld, rd, ru, lu};
  }

private:
  DVector3 m_min = DVector3{};
  DVector3 m_max = DVector3{};
};

}

#endif /// OPGS16_HELPER_AABB_INFO_BOX_H