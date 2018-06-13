
///
/// @file Element/Internal/model_vector.cc
///
/// @brief
///
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-12 Create file
///

#include <Element/Internal/mesh_vector.h>

namespace opgs16::element {

DMeshVector::DMeshVector(const DVector3& position,
                         const DVector3& normal,
                         const DVector3& tangent,
                         const DVector2& texture_coord) noexcept :
  m_position { position },
  m_normal { normal },
  m_tangent { tangent },
  m_texture_coord { texture_coord } {}

} /// ::opgs16::element