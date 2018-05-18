#ifndef OPGS16_SYSTEM_ELEMENT_INTERNAL_DIRECTION_TYPE_H
#define OPGS16_SYSTEM_ELEMENT_INTERNAL_DIRECTION_TYPE_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Impl/object_impl.h
 * @brief Direction enum file.
 * @log
 * 2018-04-18 Create file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <array>    /*! std::array */

namespace opgs16 {
namespace element {
namespace _internal {

enum class EDirection { X, Y, Z };
constexpr std::array<EDirection, 3> k_direction_list{ EDirection::X, EDirection::Y, EDirection::Z };

}
}
}

#endif // OPGS16_SYSTEM_ELEMENT_INTERNAL_DIRECTION_TYPE_H