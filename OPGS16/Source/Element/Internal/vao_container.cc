
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/Internal/vao_container.cc
///
/// @brief
/// Definition file of CVaoContainer.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-06-14 Create file.
///

/// Header file
#include <Element/Internal/vao_container.h>

namespace opgs16::element {

CVaoContainer::CVaoContainer(const std::string& model_name,
                             const TVaoList& vao_list) :
    m_vao_list(vao_list), m_container_name(model_name) {}

} /// ::opgs16::element namespace