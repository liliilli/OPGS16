#include "object_manager.h"
#include <stack>            /*! std::stack */
#include "scene_manager.h"  /*! SceneManager */
#include <type_traits>

void ObjectManager::Destroy(const Object& object) {
    auto hash_value = object.GetHash();

    std::stack<std::unordered_map<std::string, object_ptr>*> tree_list;
    using it_type = std::remove_pointer_t<decltype(tree_list)::value_type>::iterator;
    std::stack<it_type> it_list;

    tree_list.push(&SceneManager::GetInstance().GetPresentScene()->GetObjects());
    it_list.push(tree_list.top()->begin());

    bool destroyed = false;
    while (!(destroyed || tree_list.empty())) {
        auto& object_list = *tree_list.top();
        auto it = it_list.top(); // 진행된 시점에서 다시 시작해야함.
        it_list.pop();

        for (; it != object_list.end(); ++it) {
            auto target_hash_value = it->second->GetHash();
            if (hash_value == target_hash_value) {
                AddDestroyObject(std::move(it->second));
                object_list.erase(it);

                destroyed = true;
                break;
            }
            else {
                auto& additional_list = it->second->GetChildren();
                if (!additional_list.empty()) {
                    it_list.push(++it);
                    tree_list.push(&additional_list);
                    it_list.push(additional_list.begin());
                    break;
                }
            }
        }

        if (destroyed == false && it == object_list.end()) {
            tree_list.pop();
        }
    }
}
