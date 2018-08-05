#ifndef OPGS16_ELEMENT_BUILTIN_MODEL_2DPOINT_H
#define OPGS16_ELEMENT_BUILTIN_MODEL_2DPOINT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_point.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-05 Create file
///

#include <Element/Internal/model_object.h>

namespace opgs16::builtin::model {

///
/// @class BModelPoint
///
/// @brief
/// Built-in model just stores only one empty vertex, (0, 0, 0)
///
class BModelPoint final : public opgs16::element::DModelObject {
public:
  BModelPoint();
  static constexpr const char* s_model_name = "opbtPoint";
};

} /// ::opgs16::builtin::model namespace

#endif /// OPGS16_ELEMENT_BUILTIN_MODEL_2DPOINT_H