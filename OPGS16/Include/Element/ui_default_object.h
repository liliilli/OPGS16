#ifndef OPGS16_ELEMENT_DEFAULT_UIOBJECT_H
#define OPGS16_ELEMENT_DEFAULT_UIOBJECT_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Element/default_uiobject.h
///
/// @brief
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-18 Create file
///

#include <Element/ui_object.h>

namespace opgs16::element {

class CUiDefaultObject : public opgs16::element::UiObject {
private:
  void LocalUpdate() override final;
};

} /// ::opgs16::element namespace

#endif /// OPGS16_ELEMENT_DEFAULT_UIOBJECT_H