#ifndef OPGS16_OBJECTS_TD_TEST_START_TEXT_COPY_H
#define OPGS16_OBJECTS_TD_TEST_START_TEXT_COPY_H

#include "..\..\GlobalObjects\Canvas\text.h"    /*! Canvas::Text */

class TestStartTCopy : public canvas::Text {
public:
    TestStartTCopy() :
        canvas::Text{ "Copyright (c) 2018, Jongmin Yun All rights reserved",
                      glm::vec3{16, 16, 0} } {
        SetOrigin(IOriginable::Origin::DOWN_LEFT);
        SetFontSize(8u);
    };
};

#endif // !OPGS16_OBJECTS_TD_TEST_START_TEXT_COPY_H
