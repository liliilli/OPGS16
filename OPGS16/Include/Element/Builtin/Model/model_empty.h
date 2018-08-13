#ifndef OPGS16_ELEMENT_BUILTIN_MODEL_MODEL_EMPTY_H
#define OPGS16_ELEMENT_BUILTIN_MODEL_MODEL_EMPTY_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/Builtin/model/model_empty.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-13 Create file
///

#include <Element/Internal/model_object.h>

namespace opgs16::builtin::model {

///
/// @class BModelEmpty
///
/// @brief
/// Bulit-in instance which creates x, y coordinate 2d quad model
/// with ebo.
///
class BModelEmpty final : public opgs16::element::DModelObject {
public:
  BModelEmpty();
  static constexpr const char* s_model_name = "opbtEmtpy";

private:
};

} /// ::opgs16::builtin::model namespace

#endif /// OPGS16_ELEMENT_BUILTIN_MODEL_MODEL_EMPTY_H