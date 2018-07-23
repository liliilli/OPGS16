#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/aabb_infobox.h
///

#include <Helper/aabb_infobox.h>

#include <LinearMath/btVector3.h>

namespace opgs16 {

DAABBInfoBox::DAABBInfoBox(const btVector3& min, const btVector3& max) {
  m_min = DVector3{min.getX(), min.getY(), min.getZ()};
  m_max = DVector3{max.getX(), max.getY(), max.getZ()};
}

DAABBInfoBox::DAABBInfoBox(const opgs16::DVector3& min,
                           const opgs16::DVector3& max) :
    m_min{min}, m_max{max} {}

} /// ::opgs16 namespace