#pragma once
#include "../../../../System/Element/Public/object.h"

class ObstacleBlock final : public opgs16::element::CObject {
public: ObstacleBlock();
private:
    virtual void Render() override final;

    ShaderWrapper* m_wrapper;
};
