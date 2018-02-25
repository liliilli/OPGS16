#ifndef OPGS16_OBJECTS_TD_TEST_IMAGES_H
#define OPGS16_OBJECTS_TD_TEST_IMAGES_H

/*!
 * @file
 * @brief
 */

#include <memory>                               /*! std::unique_ptr<> */
#include "..\..\GlobalObjects\Canvas\image.h"   /*! Image as base class */
#include "..\..\Headers\Fwd\objectfwd.h"        /*! Canvas::canvas */

/*!
 * @class TestImage
 * @parent Canvas::Image
 * @brief
 */
class TestImage : public canvas::Image {
public:
    TestImage(const std::unique_ptr<canvas::Canvas>& canvas);

private:

};

#endif // !OPGS16_OBJECTS_TD_TEST_IMAGES_H
