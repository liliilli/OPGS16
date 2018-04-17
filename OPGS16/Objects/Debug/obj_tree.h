#ifndef OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h
#define OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h

#include "../../System/Element/Canvas/Public/text.h" /*! ::opgs16::element:canvas::CText */
#include "../../Headers/Fwd/objectfwd.h"        /*! ObjecTree */

class ObjectObjectTree : public opgs16::element::canvas::CText {
public:
    ObjectObjectTree() : CText{ "", glm::vec3{16, -32, 0} } {
        SetColor({ 1, 1, 1 });
		SetOrigin(IOriginable::Origin::UP_LEFT);
        SetFontName("Solomon");
        SetFontSize(8u);
    }
};

#endif // !OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h