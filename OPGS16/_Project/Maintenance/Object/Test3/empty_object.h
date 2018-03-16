#pragma once
#include "../../../../System/Element/Public/object.h"

class EmptyObject final : public opgs16::element::CObject {
public:     EmptyObject() {};
private:    virtual void Render() override final {};
};