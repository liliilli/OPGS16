#include "scene_manager.h"

#include "physics_manager.h"    /*! PhysicsManager */
#include "timer_manager.h"      /*! TimerManager */
#include "sound_manager.h"      /*! SoundManager */
#include "..\Shader\shader_manager.h"   /*! ShaderManager */
#include "texture_manager.h"    /*! TextureManager */
#include "object_manager.h"     /*! ObjectManager */

void SceneManager::PopScene() {
    if (!m_scenes.empty())
        m_scenes.pop();
}

void SceneManager::ReleaseAllResources() {
    PhysicsManager::GetInstance().Clear();  /*! precise */
    TimerManager::GetInstance().Clear();    /*! precise */
    SoundManager::GetInstance().Clear();    /*! Not precise */
    ShaderManager::GetInstance().Clear();   /*! Not implemented */
    TextureManager::GetInstance().Clear();  /*! Not precise? */
    ObjectManager::GetInstance().Clear();   /*! Not precise? */
}

SceneManager::~SceneManager() = default;