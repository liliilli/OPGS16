#ifndef OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H

/*!
 * @file System\Manager\physics_manager.h
 * @brief
 * @author Jongmin Yun
 * @date 2018-02-15
 */

#include <list>
#include <memory>       /*! std::unique_ptr */
#include <vector>       /*! std::list */
#include <glm\glm.hpp>  /*! glm::vec2 */
#include "..\..\Headers\Fwd\objectfwd.h"    /*! component::Rigidbody2D
                                              * collision::Collider2D */

/*!
 * @class PhysicsManager
 */
class PhysicsManager final {
public:
    /*!
     * @brief
     * @return
     */
    static PhysicsManager& GetInstance() {
        static PhysicsManager instance{};
        return instance;
    }

    /*!
     * @brief
     * @param[in]
     */
    void AddCollider(collision::RectangleCollider2D* const collider,
                     component::Rigidbody2D* const rigidbody);

    /*! Update and proceed collision routine */
    void Update();

    /*! Release and clean physics object container */
    void Clear();

private:
    struct Item {
        collision::RectangleCollider2D* const    m_collider;
        component::Rigidbody2D* const   m_rigidbody;
        const glm::vec2 m_position;

        enum class Type {
            BEGIN,
            END
        } m_type = { Type::BEGIN };

        explicit Item(collision::RectangleCollider2D* const collider,
                      component::Rigidbody2D* const rigidbody,
                      const glm::vec2 axis_value,
                      Type type) :
            m_collider{ collider }, m_rigidbody{ rigidbody },
            m_position{ axis_value }, m_type{ type } {};
    };

    using item_ptr = std::unique_ptr<Item>;
    using item_raw = Item * ;
    std::vector<item_ptr> m_potential;
    std::vector<item_raw> m_active;

private:
    /*!
     * @brief
     * @param[in]
     */
     void ProceedCollisionCheck(item_ptr& item);

    /*!
     * @brief
     * @param[in]
     */
     void EraseItem(item_ptr& item);
};

#endif // !OPGS16_SYSTEM_MANAGER_PHYSICS_MANAGER_H
