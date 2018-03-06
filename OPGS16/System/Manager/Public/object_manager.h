#ifndef OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System\Manager\object_manager.h
 * @brief
 *
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring.
 * 2018-03-05 Add Render function and rendering layer container.
 */

#include <memory>                   /*! std::unique_ptr */
#include <list>                     /*! std::list */
#include <vector>                   /*! std::vector */
#include "../../../Headers/Fwd/objectfwd.h"

namespace opgs16 {
namespace manager {

/*!
 * @class ObjectManager
 * @brief
 */
class ObjectManager final {
    using object_ptr = std::unique_ptr<Object>;
    using object_raw = Object * ;

public:
    static ObjectManager& Instance() {
        static ObjectManager instance{};
        return instance;
    }

    inline void Update() {
        if (!m_destroy_candidates.empty()) {
            DestroyObjects();
        }
    }

    void Destroy(const Object& object);

    void InsertRenderingObject(object_raw const object);

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

    ObjectManager();

public:
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

inline void Clear(ObjectManager& manager) {
    manager.ClearDestroyCandidates();
    manager.ClearRenderingList();
}

} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_OBJECT_MANAGER_H
