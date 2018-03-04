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
 * @file System/Manager/Private/object_manager.cc
 * @author Jongmin Yun
 * @log
 * 2018-03-04 Refactoring.
 */

#include "../Public/object_manager.h" /*! Header file */

#include <stack>                /*! std::stack */

#include "../Public/scene_manager.h"   /*! SceneManager */

namespace opgs16 {
namespace manager {

void ObjectManager::Destroy(const Object& object) {
    const auto hash_value = object.GetHash();

    using object_map    = std::unordered_map<std::string, object_ptr>;
    using it_type       = object_map::iterator;
    std::stack<object_map*> tree_list;
    std::stack<it_type> it_list;

    tree_list.emplace(&SceneManager::Instance().PresentScene()->GetObjectList());
    it_list.emplace(tree_list.top()->begin());

    auto destroyed = false;
    while (!(destroyed || tree_list.empty())) {
        auto& object_list = *tree_list.top();
        auto it = it_list.top(); it_list.pop();

        for (; it != object_list.end(); ++it) {
            if (it->second) {   /*! If it is empty */
                if (hash_value == it->second->GetHash()) {
                    AddDestroyObject(it->second);
                    destroyed = true;
                    break;
                }

                if (auto& additional_list = it->second->GetChildList(); !additional_list.empty()) {
                    it_list.emplace(++it);
                    tree_list.emplace(&additional_list);
                    it_list.emplace(additional_list.begin());
                    break;
                }
            }
        }

        if (!destroyed && it == object_list.end()) {
            tree_list.pop();
        }
    }
}

void ObjectManager::Clear() {
    m_destroy_candidates.clear();
}

void ObjectManager::DestroyObjects() {
    for (auto& object : m_destroy_candidates) {
        auto hash_value = object->GetHash();

        using object_map    = std::unordered_map<std::string, object_ptr>;
        using it_type       = object_map::iterator;
        std::stack<object_map*> tree_list;
        std::stack<it_type> it_list;

        tree_list.emplace(&SceneManager::Instance().PresentScene()->GetObjectList());
        it_list.emplace(tree_list.top()->begin());

        bool destroyed = false;
        while (!(destroyed || tree_list.empty())) {
            auto& object_list = *tree_list.top();
            auto it = it_list.top();
            it_list.pop();

            for (; it != object_list.end(); ++it) {
                if (!(it->second)) {
                    object_list.erase(it);
                    destroyed = true;
                    break;
                }

                if (auto& additional_list = it->second->GetChildList(); !additional_list.empty()) {
                    it_list.emplace(++it);
                    tree_list.emplace(&additional_list);
                    it_list.emplace(additional_list.begin());
                    break;
                }
            }

            if (!destroyed && it == object_list.end()) {
                tree_list.pop();
            }
        }
    }

    m_destroy_candidates.clear();
}

} /*! opgs16::manager */
} /*! opgs16 */
