#ifndef OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H
#define OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/postprocessing_manager.h
/// @brief Elementary manager class to manage post-processing shaders.
///
/// This file consists of application operation class and member API functions.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-02-18 Create file.
/// 2018-03-04 Refactoring. Move class to /Manager class.
/// 2018-04-20 Refactoring. CFrameBufferFrame to ::opgs16::element::CFrameBufferFrame.
/// 2018-06-15 Create file.
///

#include <memory>
#include <string>
#include <unordered_map>

/// ::phitos::enums::EFound.
#include <Phitos/Enums/found.h>
/// ::phitos::enums::ESucceed
#include <Phitos/Enums/success.h>
/// ::
#include <Headers/import_logger.h>
/// ::opgs16::element::CFrameBufferFrame
#include <Shader/framebuffer.h>

//!
//! Forward declarations.
//!

namespace opgs16::manager::postprocessing {

using TPpEffectPtr  = std::unique_ptr<element::CPostProcessingFrame>;
using TEffectMap    = std::unordered_map<std::string, TPpEffectPtr>;
using TSequenceType = std::list<element::CPostProcessingFrame*>;

} /// ::opgs16::manager::postprocessing

namespace opgs16::manager::postprocessing::__ {

///
/// @brief Initiate function.
/// This function must not be called twice.
///
void Initiate();

///
/// @brief Shutdown and release all resource of pp.
/// This function must not be called twice.
///
void Shutdown();

///
/// @brief Get internal reference of effect map container.
///
TEffectMap& Get() noexcept;

} /// ::opgs16::manager::postprocessing::__ namespace

///
/// @namespace postprocessing
///
/// @brief
/// This class manages post-processing frame (instances) to be used in Application.
/// Each post-processing instance stored in container could be attached, released, destroyed in
/// any time but Strongly recommends it in Initiate time and Update time.
///
namespace opgs16::manager::postprocessing {

///
/// @brief Get lvalue reference of effect which is named with tag.
/// @param[in] effect_name
/// @return Lvalue reference of effect.
///
std::pair<element::CPostProcessingFrame*, phitos::enums::EFound>
GetEffect(const std::string& effect_name);

///
/// @brief Find whether or not effect named tag is exist.
/// @param[in] effect_name The name to find effect.
/// @return If searching effect is successful, return true. else return false.
///
phitos::enums::EFound IsEffectExist(const std::string& effect_name);

///
/// @brief Insert effect (vacant) instance into effect container.
/// Type paramter must be written in <> and which must derive PostProcessingFrame base class.
///
/// @param[in] effect_name Postprocessing effect name to use later.
/// @tparam TTy Post-processing Effect type parameter to use.
///
/// @return If this method success to create and insert _Ty effect, return True.
///
template <
    class TTy,
    typename = std::enable_if_t<
        std::is_base_of_v<element::CPostProcessingFrame,
                          TTy>
    >
>
phitos::enums::ESucceed InsertEffect(const std::string& effect_name) {
  using phitos::enums::ESucceed;

  if (IsEffectExist(effect_name) == phitos::enums::EFound::Found) {
    PUSH_LOG_WARN_EXT("Postprocessing effect is already inserted. [{} : {}]",
        "Effect name", effect_name);
    return ESucceed::Failed;
  }

  auto [it, result] = __::Get().try_emplace(effect_name, std::make_unique<TTy>());
  if (!result) {
    PUSH_LOG_ERROR_EXT("Postprocessing effect is not inserted properly. [{} : {}]",
        "Effect name", effect_name);
    return ESucceed::Failed;
  }

  return ESucceed::Succeed;
}

///
/// @brief Overloading version of InsertEffect<_Ty, ...>(tag)
/// @param[in] effect_name
///
phitos::enums::ESucceed InsertEffect(const std::string& effect_name);

///
/// @brief Insert effect and initiate automatically.
/// Type paramter must be written in <> and which must derive PostProcessingFrame base class.
///
/// @param[in] effect_name Postprocessing effect name to use later.
/// @tparam TTy Post-processing Effect type parameter to use.
///
/// @return If this method success to create and insert _Ty effect, return True.
///
template <
    class TTy,
    typename = std::enable_if_t<
        std::is_base_of_v<element::CPostProcessingFrame, TTy>
    >
>
phitos::enums::ESucceed InsertEffectInitiate(const std::string& effect_name) {
  if (InsertEffect<TTy>(effect_name) == phitos::enums::ESucceed::Failed) {
    PUSH_LOG_ERROR_EXT("Failed to insert post-processing effect. [{} : {}]",
        "Effect name", effect_name);
    return phitos::enums::ESucceed::Failed;
  }

  __::Get()[effect_name]->Initialize();
  PUSH_LOG_INFO_EXT("Succeed to insert post-processing effect. [{} : {}]",
      "Effect name", effect_name);
  return phitos::enums::ESucceed::Succeed;
}

///
/// @brief Set continous post-processing sequence to render screen.
/// If sequence list on id is already set up, This method does nothing.
///
/// @param[in] sequence_name name which is position to be set effect sequences.
/// @param[in] list Postprocessing effect tags list
///
/// @return Pointer of initialized effect sequence.
/// If this fails to create sequence, return nullptr and success flag.
///
std::pair<TSequenceType*, phitos::enums::ESucceed>
SetSequence(const std::string& sequence_name,
            const std::initializer_list<std::string>& list);

///
/// @brief
/// Updates each effects of each sequences. (each effects of sequence is active)
///
void UpdateSequences();

///
/// @brief Bind effect sequence with id number.
///
/// @param[in] sequence_name
/// Name position of effect sequences container to bind seqeunce.
///
/// @return If bind sequence is successful, return ESucceed::Success.
///
phitos::enums::ESucceed BindSequence(const std::string& sequence_name);

///
/// @brief Release only list of post-processing sequence called sequence_name.
/// @param[in] sequence_name Sequence name to find.
/// @return If successful, return ESucceed::Success; otherwise ESucceed::Failed.
///
phitos::enums::ESucceed ReleaseSequence(const std::string& sequence_name);

///
/// @brief Render post-processing sequence.
///
void Render();

} /// ::opgs16::manager::postprocessing namespace

#endif /// OPGS16_S_SYSTEM_SHADER_POST_PROCESSING_MANAGER_H
