#ifndef TEMPLATE_HELLO_WORLD_H
#define TEMPLATE_HELLO_WORLD_H

#include <Element/scene.h>

class SceneFirstHelloWorld final : public opgs16::element::CScene {
public:
  void Initiate() override final;
};

#endif /// TEMPLATE_HELLO_WORLD_H