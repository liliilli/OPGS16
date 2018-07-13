#ifndef MAGICCUB_OBJECT_SCENEGAMEPLAY_OBJ_STAGE_H
#define MAGICCUB_OBJECT_SCENEGAMEPLAY_OBJ_STAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/object.h>

namespace magiccup {

class StageObjectManagement final : public opgs16::element::CObject {
public:
  StageObjectManagement();

  inline static constexpr const char* s_object_name = "ObjMan";
};

} /// ::magiccup namespace

#endif /// MAGICCUB_OBJECT_SCENEGAMEPLAY_OBJ_STAGE_H