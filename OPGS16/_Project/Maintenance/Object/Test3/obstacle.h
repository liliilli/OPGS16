#pragma once
#include "../../../../System/Element/Public/object.h"

class ObstacleBlock final : public opgs16::element::CObject {
public: ObstacleBlock();
private:
    virtual void Render() override final;
    virtual void OnCollisionEnter(opgs16::component::CRigidbody2D& collider) override final;

    ShaderWrapper* m_wrapper;
};
