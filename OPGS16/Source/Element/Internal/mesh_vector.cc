
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

DMeshVector::DMeshVector(
    const std::pair<DVector3, phitos::enums::EActivated>& position,
    const std::pair<DVector3, phitos::enums::EActivated>& normal,
    const std::pair<DVector3, phitos::enums::EActivated>& tangent,
    const std::pair<DVector2, phitos::enums::EActivated>& texture_coord)
    noexcept :
    DMeshVector(position.first, normal.first,
                tangent.first, texture_coord.first) {

}

} /// ::opgs16::element