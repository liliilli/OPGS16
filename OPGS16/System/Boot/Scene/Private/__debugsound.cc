
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Boot/Scene/Private/__debugsound.cc
* @brief
* Implementation file of ../Public/__debugsound.h
*
* @log
* 2018-05-13 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__debugsound.h" /*! Header file */
#include "../../Object/___Debug/Public/__s_debugcanvas.h"

namespace opgs16::builtin::sample {

void __DEBUGSOUND::Initiate() {
    Instantiate<__S_DebugTestCanvas>("DebugTestCanvas");
};

} /*! opgs16::builtin::sample */
