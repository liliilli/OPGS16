#ifndef OPGS16_ELEMENT_BUILTIN_EMPTY_CANVAS_H
#define OPGS16_ELEMENT_BUILTIN_EMPTY_CANVAS_H

/// ::opgs16::element::canvas::CCanvas
#include <Element/Canvas/canvas.h>

namespace opgs16::builtin {

class EmptyCanvas final : public opgs16::element::canvas::CCanvas {
public:
  inline static constexpr const char* sObjectName = "Canvas";
};

} /// ::opgs16::builtin namespace

#endif /// OPGS16_ELEMENT_BUILTIN_EMPTY_CANVAS_H