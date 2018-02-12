#include "obj_fps.h"
#include <iomanip>  /*! std::setprecision */
#include <sstream>  /*! std::ostringstream */
#include "..\..\System\Manager\time_manager.h"  /*! TimeManager */

void ObjectFps::Update() {
    std::ostringstream str;
    str << std::setprecision(4) << TimeManager::GetInstance().GetFpsSeconds();
    SetText("Fps : " + str.str());
}
