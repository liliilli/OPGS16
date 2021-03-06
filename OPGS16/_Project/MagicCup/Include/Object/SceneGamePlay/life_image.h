#ifndef MAGICCUP_OBJECT_SCENEGAMEPLAY_LIFE_IMAGE_H
#define MAGICCUP_OBJECT_SCENEGAMEPLAY_LIFE_IMAGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Element/Canvas/image.h>

namespace magiccup {

class LifeImage final : public opgs16::element::canvas::CImage {
public:
  LifeImage(const opgs16::element::canvas::CCanvas* canvas_reference);

};

} /// ::magiccup namespace

#endif /// MAGICCUP_OBJECT_SCENEGAMEPLAY_LIFE_IMAGE_H