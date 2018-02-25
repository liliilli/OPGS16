#ifndef OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h
#define OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h

#include "..\..\GlobalObjects\Canvas\text.h"    /*! Canvas::Text */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! ObjecTree */

class ObjectObjectTree : public canvas::Text {
public:
    ObjectObjectTree() : canvas::Text{ "", glm::vec3{16, -32, 0} } {
        SetColor({ 1, 1, 1 });
		SetOrigin(IOriginable::Origin::UP_LEFT);
        SetFontName("Solomon");
        SetFontSize(8u);
    }
};

#endif // !OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h