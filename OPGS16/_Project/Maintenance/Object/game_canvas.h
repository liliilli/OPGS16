#pragma once

#include "..\..\GlobalObjects\Canvas\canvas.h"  /*! Canvas::Canvas */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! component::ScriptFrame */

class GameCanvas : public canvas::Canvas {
public:
	GameCanvas();
	virtual ~GameCanvas() = default;
};