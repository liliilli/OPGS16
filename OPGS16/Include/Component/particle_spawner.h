#ifndef OPGS16_COMPONENT_PARTICLE_RENDERER_H
#define OPGS16_COMPONENT_PARTICLE_RENDERER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Component/particle_renderer.h
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-03 Create file.
/// 2018-08-09 Add functionality.
///

#include <Component/Internal/component.h>
#include <Component/particle_emitter.h>

namespace opgs16::component {

class CParticleSpawner final : public opgs16::component::_internal::CComponent {
  using TEmitterSmtPtr = std::unique_ptr<CParticleEmitter>;
  using TEmitterContainer = std::unordered_map<std::string, TEmitterSmtPtr>;

public:
  CParticleSpawner(element::CObject& bind_object) : CComponent{bind_object} {};
  ~CParticleSpawner();

  CParticleSpawner(const CParticleSpawner&) = default;
  CParticleSpawner(CParticleSpawner&&) = default;
  CParticleSpawner& operator=(const CParticleSpawner&) = default;
  CParticleSpawner& operator=(CParticleSpawner&&) = default;

  ///
  /// @brief Create empty emitter with name called emitter_name, and bind it spawner.
  /// @param[in] emitter_name Emitter name.
  ///
  CParticleEmitter* CreateEmptyParticleEmitter(const std::string& emitter_name);

  ///
  /// @brief Create and bind derived emitter to spawner to be executed.
  /// @tparam TEmitterType Derived emitter user class type.
  ///
  template <
    typename TEmitterType,
    typename = std::enable_if_t<std::is_base_of_v<CParticleEmitter, TEmitterType>>
  >
  CParticleEmitter* CreateParticleEmitter() {
    const char* name = OP16_GET_LITERAL_NAME(TEmitterType);
    if (GetParticleEmitter(name)) {
      PUSH_LOG_ERROR_EXT("Could not create particle emitter, {}.", name);
      return nullptr;
    }

    auto [it, result] = m_emitter.try_emplace(name, std::make_unique<TEmitterType>(GetBindObject()));
    if (!result) {
      PUSH_LOG_ERROR_EXT("Could not create particle emitter, {}. Unexpected Error.", name);
      PHITOS_ASSERT(result, "Could not create particle emitter. Halt program.");
      return nullptr;
    }
    return it->second.get();
  }

  ///
  /// @brief
  ///
  CParticleEmitter* GetParticleEmitter(const std::string& emitter_name);

  ///
  /// @brief
  ///
  bool DestroyParticleEmitter(const std::string& emitter_name);

  ///
  /// @brief
  ///
  bool IsSleep() noexcept;

  ///
  /// @brief
  ///
  void StartAll() noexcept;

private:
  void Update(float delta_time) override final;

  TEmitterContainer m_emitter;
  bool m_is_removed_from_original_object = false;

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CParticleSpawner)
};

} /// ::opgs16::component namespace

#endif /// OPGS16_COMPONENT_PARTICLE_RENDERER_H