#ifndef OPGS16_OBJECTS_DB_DEBUG_CANVAS_H
#define OPGS16_OBJECTS_DB_DEBUG_CANVAS_H

/**
 * @file Object\Debug\debug_canvas.h
 * @brief Canvas frame object to display UI components.
 *
 * @author Jongmin Yun
 * @date 2018-02-13
 */

#include <vector>                               /*! std::vector */
#include "..\..\GlobalObjects\Canvas\canvas.h"  /*! Canvas::Canvas */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! component::ScriptFrame */

/*!
 * @class CanvasDebug
 * @brief temporary
 */
class CanvasDebug : public Canvas::Canvas {
public:
	/**
	 * @brief Canvas constructor.
	 * Canvas has orthographic camera fixed to screen size, this camera will be initialized.
	 */
	CanvasDebug();
	virtual ~CanvasDebug() = default;
};

#endif /** OPGS16_OBJECTS_DB_DEBUG_CANVAS_H */