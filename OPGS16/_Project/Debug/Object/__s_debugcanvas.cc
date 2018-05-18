
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "__s_debugcanvas.h"    /// Header file
#include "../Script/canvas_script.h"

namespace opgs16::builtin::sample {

__S_DebugTestCanvas::__S_DebugTestCanvas() {
    AddComponent<DebugCanvasScript>(*this);
}

} /*! opgs16::builtin::sample */

