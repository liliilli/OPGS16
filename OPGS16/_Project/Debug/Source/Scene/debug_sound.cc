
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Boot/Scene/Private/__debugsound.cc
* @brief
* Implementation file of ../Public/__debugsound.h
*
* @log
* 2018-05-13 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "..\..\Include\Scene\debug_sound.h"   /// Header file
#include "..\..\Include\Object\__s_debugcanvas.h"

namespace opgs16::builtin::sample {

void __DEBUGSOUND::Initiate() {
    Instantiate<__S_DebugTestCanvas>("DebugTestCanvas");
};

} /*! opgs16::builtin::sample */
