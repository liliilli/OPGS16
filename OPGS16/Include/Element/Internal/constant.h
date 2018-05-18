#ifndef OPGS16_SYSTEM_ELEMENT_INTERNAL_CONSTANT_H
#define OPGS16_SYSTEM_ELEMENT_INTERNAL_CONSTANT_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Impl/constant.h
 * @brief Constant values file used in framebuffer.h
 * @log
 * 2018-04-21 Create file. Move values into this file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <array>    /*! std::array */

namespace opgs16::element::_internal {

constexpr std::array<int, 4> screen_coord{ 0, 0, 256, 224 };
constexpr std::array<unsigned, 6> quad_indices = { 0, 1, 2, 2, 3, 0 };
constexpr std::array<float, 32> quad_info = {
	// Vertex       //Normal        // TexCoord
	1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 1.f,
	1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  1.f, 0.f,
	-1.f,-1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 0.f,
	-1.f, 1.f, 0.f,  0.f, 0.f, 1.f,  0.f, 1.f
};

} /*! opgs16::element::_internal */

#endif // OPGS16_SYSTEM_ELEMENT_INTERNAL_CONSTANT_H