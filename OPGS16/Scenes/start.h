#ifndef OPENGL_TUTORIAL_SCENES_START_H
#define OPENGL_TUTORIAL_SCENES_START_H
#include "..\System\Frame\scene.h"
#include "..\System\Object\object.h"

class Start : public Scene {
public:
    virtual ~Start() = default;

    // Inherited via Scene
    virtual void Initiate() override final;

    /**
    * @brief The method update components movement, UI refresh, and so on.
    */
    [[noreturn]] virtual void Update() override;

    /**
    * @brief The method calls scene to draw all objects.
    */
    [[noreturn]] virtual void Draw() override;

};

#endif /** OPENGL_TUTORIAL_SCENES_START_H */