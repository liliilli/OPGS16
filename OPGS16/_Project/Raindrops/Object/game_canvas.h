#ifndef OPGS16_OBJECTS_DB_DEBUG_CANVAS_H
#define OPGS16_OBJECTS_DB_DEBUG_CANVAS_H

#include "..\..\GlobalObjects\Canvas\canvas.h"  /*! Canvas::Canvas */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! component::ScriptFrame */

class GameCanvas : public Canvas::Canvas {
public:
	GameCanvas();
	virtual ~GameCanvas() = default;
};

#endif /** OPGS16_OBJECTS_DB_DEBUG_CANVAS_H */