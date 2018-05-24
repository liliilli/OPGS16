#ifndef OPGS16_COMPONENT_INTERNAL_SCRIPT_FRAME_FRAG_H
#define OPGS16_COMPONENT_INTERNAL_SCRIPT_FRAME_FRAG_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Components\Internal\script_frame_flag.h
///
/// @brief
/// Boolean flag enum class container for opgs16::component::CScriptFrame.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-05-24 Create file.
///

namespace opgs16::component::_internal {

///
/// @enum EScriptInitiated
/// @brief
/// This enum class is for checking and preventing duplicated
/// ::opgs16::component::CScriptFrame::Initiate() call from somewhere.
///
enum class EScriptInitiated : bool {
  NotInitiated = false,
  Initiated = true
};

///
/// @enum EScriptStarted
/// @brief
/// This enum class is for checking and preventing arbitary calling of Start()
/// function from anywhere of source code.
///
enum class EScriptStarted : bool {
  NotStarted = false,
  Started = true
};

} /// ::opgs16::component::_internal

#endif /// OPGS16_COMPONENT_INTERNAL_SCRIPT_FRAME_FRAG_H