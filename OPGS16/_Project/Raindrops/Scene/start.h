#ifndef OPGS16__SCENES_START_H
#define OPGS16__SCENES_START_H
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
     virtual void Update() override;

    /**
    * @brief The method calls scene to draw all m_object_list.
    */
     virtual void Draw() override;

};

#endif /** OPGS16__SCENES_START_H */
