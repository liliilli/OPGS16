#pragma once

#include "..\..\..\System\Frame\scene.h"    /*! Scene */

class __BOOT : public Scene {
public:
    virtual void Initiate() override final;

    virtual void Draw() override final;
};