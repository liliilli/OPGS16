#ifndef OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H
#define OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/Internal/particle_emitter.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-03 Create file.
///

#include <Component/sprite2d_renderer.h>
#include <Component/Internal/renderer_base.h>
#include <Component/Internal/particle_renderer.h>
#include <Component/ParticleModule/module_spawn.h>
#include <Element/Internal/particle_object.h>
#include <Helper/Type/vector3.h>

//!
//! Forward declaration
//!

namespace opgs16::component {
class CParticleSpawner;
class CParticleModuleSpawn;
}

//!
//! Implementation
//!

namespace opgs16::component {

///
/// @struct DParticleInitialData
/// @brief
///
struct DParticleInitialData {
  int32_t m_lifetime      = 0;
  int32_t m_initial_size  = 0;
  DColor  m_initial_color = DColor::White;
  float   m_initial_alpha = 1.f;
  DVector3 m_initial_velocity;
  DVector3 m_initial_accelation;
  DVector3 m_initial_postion;

  bool m_is_lifetime = false;
  bool m_is_initial_size = false;
  bool m_is_initial_color = false;
  bool m_is_initial_alpha = false;
  bool m_is_initial_velocity = false;
  bool m_is_initial_accelation = false;

  bool m_is_bylife_size = false;
  bool m_is_bylife_alpha = false;
};

///
/// @class CParticleEmitter
///
class CParticleEmitter : public _internal::CRendererBase {
  using CParticleObject       = element::_internal::CInternalParticleObject;
  using TParticleModuleSmtPtr = std::unique_ptr<_internal::CInternalParticleModuleBase>;
  using TParitcleModuleContainer = std::unordered_map<uint32_t, TParticleModuleSmtPtr>;
public:
  CParticleEmitter(element::CObject& bind_object);

  ///
  /// @brief Check emitter is slept.
  /// Sleep flag will be true if emitter is not spawning any particles.
  ///
  bool IsSleep() const noexcept;

  ///
  /// @brief
  ///
  void SetTexture(const std::string& texture_name);

  ///
  /// @brief
  ///
  void SetTextureFragmentIndex(int32_t fragment_index);

  ///
  /// @brief
  ///
  void SetLoop(bool is_loop);

  ///
  /// @brief
  ///
  CParticleModuleSpawn& GetSpawnModuleRef();

  ///
  /// @brief
  ///
  void Render();

  ///
  /// @brief
  /// @tparam TModuleType
  /// @return
  ///
  template<
    class TModuleType,
    typename = std::enable_if_t<std::is_base_of_v<_internal::CInternalParticleModuleBase, TModuleType>>
  >
  TModuleType* AddModule() {
    auto [result_pair, result] = m_modules.try_emplace(OP16_GET_HASH(TModuleType), nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Module did not be made properly.");
      return nullptr;
    }

    result_pair->second = std::make_unique<TModuleType>(*this);
    return static_cast<TModuleType*>(result_pair->second.get());
  }

  ///
  /// @brief
  /// @tparam TModuleType
  /// @return
  ///
  template <
    class TModuleType,
    typename = std::enable_if_t<std::is_base_of_v<_internal::CInternalParticleModuleBase, TModuleType>>
  >
  TModuleType* GetModule() {
    auto it = m_modules.find(OP16_GET_HASH(TModuleType));
    if (it == m_modules.end())
      return nullptr;

    return static_cast<TModuleType*>(it->second.get());
  }

private:
  ///
  ///
  ///
  void Update(float delta_time) override final;

  ///
  /// @brief
  ///
  void pfUpdateSpawnLocationBasis(const DVector3& position);

  ///
  /// @brief
  ///
  void pUpdateCommonUniformProperties(element::CShaderWrapper& wrapper);

  ///
  /// @brief
  ///
  void pfCreateObjects(int32_t numbers);

  ///
  /// @brief
  ///
  void pfSetParticleSpawnSetting(bool is_spawn);

  ///
  /// @brief
  ///
  void pActivateObject(CParticleObject& object);

  std::vector<CParticleObject> m_object_list;
  std::forward_list<CParticleObject*> m_valid_objects;
  int32_t m_pool_last = 0;

  _internal::CInternalParticleRenderer  m_renderer;
  CParticleModuleSpawn     m_module_spawn;
  TParitcleModuleContainer m_modules;

  DVector3 m_location_basis = {};
  bool m_is_looped = false;

  friend opgs16::component::CParticleSpawner;
  friend opgs16::component::CParticleModuleSpawn;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CRendererBase, CRendererBase)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_INTERNAL_PARTICLE_EMITTER_H