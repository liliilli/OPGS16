#include "__b_canv.h"                           /*! Header file */

#include <glm\glm.hpp>
#include "..\Scripts\__b_scr.h"                 /*! __B_SCR */
#include "..\..\..\GlobalObjects\Canvas\image.h"/*! canvas::Image */

__B_CANV::__B_CANV() {
    /*! Object binding */
    Instantiate<canvas::Image>("Logo", "__B_Logo", this);
    auto text = static_cast<canvas::Image*>(GetChild("Text")); {
        text->SetOrigin(IOriginable::Origin::CENTER_CENTER);
        text->SetWorldPosition({ 0, 48, 0 });
    }


}