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

#include <Helper/vector.h>

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

private:
  DVector3 m_min = DVector3{};
  DVector3 m_max = DVector3{};
};

}

#endif /// OPGS16_HELPER_AABB_INFO_BOX_H