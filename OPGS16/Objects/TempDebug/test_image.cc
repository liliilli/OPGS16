#include "test_image.h"

TestImage::TestImage(const std::unique_ptr<canvas::Canvas>& canvas) :
    Image("Test", canvas) {
    SetImageSize(256, 224);
}
