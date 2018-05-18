#ifndef OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h
#define OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_OBJECT_TREE_h

/// ::opgs16::element::canvas::CText
#include <Element\Canvas\text.h>
#include <opgs16fwd.h>

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