#include "test_image.h"

TestImage::TestImage(const std::unique_ptr<Canvas::Canvas>& canvas) :
    Image("Test", canvas) {
    SetImageSize(256, 224);
}
