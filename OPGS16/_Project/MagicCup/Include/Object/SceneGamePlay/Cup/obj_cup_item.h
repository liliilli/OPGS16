#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_CUP_ITEM_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_CUP_ITEM_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {

class ObjectCupItem final : public opgs16::element::CObject {
public:
  ObjectCupItem();

  inline static constexpr const char* s_object_name = "CupItem";

private:
};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_CUP_OBJ_CUP_ITEM_H