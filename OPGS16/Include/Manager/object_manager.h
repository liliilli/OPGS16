#ifndef OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

///
/// @file System\Manager\object_manager.h
/// @brief
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Refactoring.
/// 2018-03-05 Add Render function and rendering layer container.
///

#include <memory>       /*! std::unique_ptr */
#include <list>         /*! std::list */
#include <vector>       /*! std::vector */

#include <opgs16fwd.h>  /// Forward declaration

namespace opgs16 {
namespace manager {

/*!
 * @class MObjectManager
 * @brief
 */
class MObjectManager final {
    using object_ptr = std::unique_ptr<element::CObject>;
    using object_raw = element::CObject * ;

public:
    static MObjectManager& Instance() {
        static MObjectManager instance{};
        return instance;
    }

    inline void Update() {
        if (!m_destroy_candidates.empty()) {
            DestroyObjects();
        }
    }

    ///
  /// @brief
  /// Destroy specified object in present scene.
    ///
  /// If the object has script component, object will call Destroy() function
  /// before deleted permanently.
    /// @param[in] object Object to be destroyed.
  ///
    void Destroy(const element::CObject& object);

    void InsertRenderingObject(object_raw const object, unsigned layer_index);

    void Render();

    /*! Clear all destroy candidates */
    void ClearDestroyCandidates() {
        m_destroy_candidates.clear();
    }

    void ClearRenderingList() {
        for (auto& sub_list : m_rendering_list)
            sub_list.clear();
    }

private:
    std::list<object_ptr> m_destroy_candidates;
    std::vector<std::list<object_raw>> m_rendering_list;

private:
    inline void AddDestroyObject(object_ptr& ptr) {
        m_destroy_candidates.emplace_back(std::move(ptr));
    }

    void DestroyObjects();

    MObjectManager();

public:
    MObjectManager(const MObjectManager&) = delete;
    MObjectManager& operator=(const MObjectManager&) = delete;
};

inline void Clear(MObjectManager& manager) {
    manager.ClearDestroyCandidates();
    manager.ClearRenderingList();
}

} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
