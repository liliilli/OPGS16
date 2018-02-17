#ifndef OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H

/*!
 * @file System\Manager\object_manager.h
 * @brief
 */

#include <memory>               /*! std::unique_ptr */
#include <list>                 /*! std::list */
#include "..\Object\object.h"   /*! Object */

/*!
 * @class
 */
class ObjectManager final {
private:
    using object_ptr = std::unique_ptr<Object>;

public:
    static ObjectManager& GetInstance() {
        static ObjectManager instance{};
        return instance;
    }

    [[noreturn]] void Destroy(const Object& object);

    [[noreturn]] void Update() {
        if (!m_destroy_candidates.empty()) {
            DestroyObjects();
        }
    }

private:
    std::list<object_ptr> m_destroy_candidates;

private:
    [[noreturn]] void AddDestroyObject(object_ptr&& ptr) {
        m_destroy_candidates.emplace_back(std::move(ptr));
    }

    [[noreturn]] void DestroyObjects();

private:
    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager(const ObjectManager&&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&&) = delete;
};

#endif // !OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
