
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
 * If you want to read full statements, read LICENSE file.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "..\..\Include\Object\__s_debugcanvas.h"    /// Header file
#include "..\..\Include\Script\canvas_script.h"

namespace opgs16::builtin::sample {

__S_DebugTestCanvas::__S_DebugTestCanvas() {
    AddComponent<DebugCanvasScript>(*this);
}

} /*! opgs16::builtin::sample */

