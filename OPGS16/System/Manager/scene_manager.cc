#include "scene_manager.h"

void SceneManager::PopScene() {
    if (!m_scenes.empty())
        m_scenes.pop();
}

