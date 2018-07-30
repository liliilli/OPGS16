#ifndef OPGS16_ELEMENT_BUILTiN_MODEL_MODEL_2D_QUADLINE_H
#define OPGS16_ELEMENT_BUILTiN_MODEL_MODEL_2D_QUADLINE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_2dquadline.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-30 Create file
///

#include <Element/Internal/model_object.h>

namespace opgs16::builtin::model {

///
/// @class BModel2DQuadLine
///
/// @brief
/// Bulit-in instance which creates x, y coordinate 2d quad model
/// with ebo.
///
class BModel2DQuadLine final : public opgs16::element::DModelObject {
public:
  BModel2DQuadLine();
  static constexpr const char* s_model_name = "opbtQuadLine";
};

}

#endif /// OPGS16_ELEMENT_BUILTiN_MODEL_MODEL_2D_QUADLINE_H