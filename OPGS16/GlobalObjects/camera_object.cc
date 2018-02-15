#include "camera_object.h"
#include "..\System\Components\camera.h"    /*! component::Camera */

MainCameraObject::MainCameraObject() {
    AddComponent<component::Camera>(
        *this,
        component::Camera::ViewType::ORTHO,
        component::Camera::CameraType::MAIN);
}
