
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/__s_debugcanvas.h"                  /*! Header file */
#include "../../../Scripts/___3/Public/canvas_script.h" /*! ::opgs16::builtin::sample::DebugCanvasScript */

namespace opgs16::builtin::sample {

__S_DebugTestCanvas::__S_DebugTestCanvas() {
    AddComponent<DebugCanvasScript>(*this);
}

} /*! opgs16::builtin::sample */

