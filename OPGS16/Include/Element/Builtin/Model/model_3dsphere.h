#ifndef OPGS16_ELEMENT_BUILTIN_MODEL_MODEL_3DSPHERE_H
#define OPGS16_ELEMENT_BUILTIN_MODEL_MODEL_3DSPHERE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_3dsphere.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-12 Create file
///

#include <Element/Internal/model_object.h>

namespace opgs16::builtin::model {

///
/// @class BModel3DSphere
///
/// @brief
/// Bulit-in instance which creates x, y coordinate 2d quad model
/// with ebo.
///
class BModel3DSphere final : public opgs16::element::DModelObject {
public:
  BModel3DSphere();
  static constexpr const char* s_model_name = "opbt3dsphere";

private:
  void pEmplaceVertex(const DVector3& normalized_position, std::vector<element::DMeshVector>& container);
  void pCreateVertexRecursively(const DVector3& a, const DVector3& b, const DVector3& c, int32_t level,
                                std::vector<element::DMeshVector>& container);
};

} /// ::opgs16::builtin::model namespace

#endif /// OPGS16_ELEMENT_BUILTIN_MODEL_MODEL_3DSPHERE_H