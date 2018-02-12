#ifndef OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_FPS_h
#define OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_FPS_h

#include "..\..\GlobalObjects\Canvas\text.h"    /*! Canvas::Text */

class ObjectFps : public Canvas::Text {
public:
    ObjectFps() : Canvas::Text{ "", glm::vec3{16, -16, 0} } {
        SetColor({ 0, 0, 0 });
		SetOrigin(IOriginable::Origin::UP_LEFT);
        SetFontName("Solomon");
        SetFontSize(8u);
    }

    [[noreturn]] virtual void Update() override final;
};

#endif // !OPGS16_OBJECTS_DEBUG_DEFAULT_OBJECT_FPS_h
