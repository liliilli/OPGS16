#pragma once

#include "..\..\GlobalObjects\Canvas\canvas.h"  /*! Canvas::Canvas */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! component::ScriptFrame */

class GameCanvas2 : public canvas::Canvas {
public:
	GameCanvas2();

private:
    glm::vec3 GetColor(unsigned index);
};