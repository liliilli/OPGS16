#include "camera_object.h"      /// Header file
#include <Component\camera.h>   /// ::opgs16::component::CCamera

MainCameraObject::MainCameraObject() {
    using Camera = opgs16::component::CCamera;
    AddComponent<Camera>(*this, Camera::ViewType::ORTHO, Camera::CameraType::MAIN);
}
