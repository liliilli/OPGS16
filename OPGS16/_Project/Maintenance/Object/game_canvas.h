#pragma once

#include "..\..\GlobalObjects\Canvas\canvas.h"  /*! Canvas::Canvas */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! component::CScriptFrame */

class GameCanvas : public canvas::Canvas {
public:
	GameCanvas();
	virtual ~GameCanvas() = default;
};