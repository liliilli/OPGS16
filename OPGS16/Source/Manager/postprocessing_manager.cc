#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Manager/postprocessing_manager.cc
///
/// @brief
///
///
/// @author Jongmin Yun
/// @log
/// 2018-03-04 Refactoring.
/// 2018-06-15 Create file.
///

/// Header file
#include <Manager/postprocessing_manager.h>

/// ::phitos:: Enhanced assertion.
#include <Phitos/Dbg/assert.h>
/// ::phitos::enums::EInitiated boolean flag.
#include <Phitos/Enums/initiated.h>

/// ::opgs16::builtin::postprocessing::PpEffectScaling
#include <Shader/PostProcessing/pp_scaling.h>

//!
//! Data
//!

namespace {

using phitos::enums::EFound;
using phitos::enums::EInitiated;
using phitos::enums::ESucceed;

using opgs16::manager::postprocessing::TPpEffectPtr;
using opgs16::manager::postprocessing::TSequenceType;

EInitiated is_management_initiated = EInitiated::NotInitiated;
EInitiated is_management_shutdowned = EInitiated::NotInitiated;

// Container sotres post-processing separated effects.
std::unordered_map<std::string, TPpEffectPtr> m_effects;

// Container stores post-processing sequences combined with effects.
std::unordered_map<std::string, TSequenceType> m_effect_sequences;
TSequenceType* m_present_sequence = nullptr;

} /// unnamed namespace

//!
//! Implementation
//!

namespace {

///
/// @brief Return id'th position of effect_sequences is already exist.
/// @param[in] sequence_name Sequence name to find.
/// @return Exist flag.
///
phitos::enums::EFound IsEffectSequenceExist(const std::string& sequence_name) {
  return m_effect_sequences.find(sequence_name) != m_effect_sequences.end() ?
      EFound::Found :
      EFound::NotFound;
}

} /// unnamed namespace

namespace opgs16::manager::postprocessing::__ {

void Initiate() {
  PHITOS_ASSERT(is_management_initiated == EInitiated::NotInitiated,
      "Duplicated management initiation is prohibited.");

  // Insert display size scaling post processing effect.
	InsertEffectInitiate<builtin::postprocessing::PpEffectScaling>("__df__");
  SetSequence("opDefault", {});

  is_management_initiated = EInitiated::Initiated;
}

void Shutdown() {
  // Release...
  m_present_sequence = nullptr;
  m_effect_sequences.clear();
  m_effects.clear();

  is_management_shutdowned = EInitiated::Initiated;
}

TEffectMap& Get() noexcept {
  return m_effects;
}

} /// ::opgs16::manager::postprocessing::__ namespace

namespace opgs16::manager::postprocessing {

std::pair<element::CPostProcessingFrame*, phitos::enums::EFound>
GetEffect(const std::string& effect_name) {
	if (IsEffectExist(effect_name) == phitos::enums::EFound::NotFound)
    return {nullptr, EFound::NotFound};

  return {m_effects[effect_name].get(), EFound::Found};
}

phitos::enums::EFound IsEffectExist(const std::string& effect_name) {
  return m_effects.find(effect_name) != m_effects.end() ?
      EFound::Found :
      EFound::NotFound;
}

phitos::enums::ESucceed InsertEffect(const std::string& effect_name) {
	return InsertEffect<element::CPostProcessingFrame>(effect_name);
}

std::pair<TSequenceType*, phitos::enums::ESucceed>
SetSequence(const std::string& sequence_name,
            const std::initializer_list<std::string>& list) {
	if (IsEffectSequenceExist(sequence_name) == EFound::Found) {
    PUSH_LOG_WARN_EXT(
        "Failed to setting sequence because of already exist in SetSequence()."
        " [{} : {}]", "Sequence name", sequence_name);
    return {&m_effect_sequences[sequence_name], ESucceed::Failed};
  }

  std::list<element::CPostProcessingFrame*>&& effect_list{};

  for (const auto& effect_name : list) {
    if (IsEffectExist(effect_name) == EFound::NotFound) {
      PUSH_LOG_ERROR_EXT(
          "Failed to create sequence some of effect is lacked. "
          "[{} : {}], [{} : {}]",
          "Sequence name", sequence_name, "Effect name", effect_name);
      return {nullptr, ESucceed::Failed};
    }

    // Push and get count up.
    effect_list.emplace_back(GetEffect(effect_name).first);
    (*effect_list.rbegin())->Active();
  }
  // Insert scaling post-processing effect,
  // for default displaying. Not have to active()
  effect_list.emplace_back(GetEffect("__df__").first);
  (*effect_list.rbegin())->Active();

  // Insert
  auto [it, result] =
      m_effect_sequences.try_emplace(sequence_name, std::move(effect_list));
  if (!result) {
    PUSH_LOG_ERROR_EXT(
        "Failed to create sequence properly. [{} : {}]",
        "Sequence name", sequence_name);
    return {&it->second, ESucceed::Failed};
  }

  PUSH_LOG_INFO_EXT(
      "Succeeded to insert post-processing sequence. ""[{} : {}]",
      "Sequence name", sequence_name);
  return {&it->second, ESucceed::Succeed};
}


void UpdateSequences() {
	for (auto& effect : m_effects) {
		auto& item = effect.second;

		if (item->IsActive())
      item->Update();
	}
}

phitos::enums::ESucceed BindSequence(const std::string& sequence_name) {
	if (IsEffectSequenceExist(sequence_name) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT(
        "Failed to bind sequence, post-processing sequence is not found. "
        "[{} : {}]", "Sequence name", sequence_name);
    return ESucceed::Failed;
  }

  m_present_sequence = &m_effect_sequences[sequence_name];
  // Bind first effect
  (*m_effect_sequences[sequence_name].begin())->Bind();
  return ESucceed::Succeed;
}

void Render() {
  // If any of effect sequence is bound, render post-processing.
	if (m_present_sequence == nullptr) {
    return;
  }

  // Iterate effect list, and rendering.
  auto it_effect = m_present_sequence->begin();
  auto it_rbegin = (--m_present_sequence->end());

  std::for_each(m_present_sequence->begin(),
                m_present_sequence->end(), [&](decltype(*it_effect)& effect) {
    // If this is not end of sequence, bind next.
    if (it_effect != it_rbegin) {
      (*std::next(it_effect))->Bind();
    }
    else {
      // Bind to default frame buffer.
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Render!
    effect->RenderEffect();
    ++it_effect;
  });
}

phitos::enums::ESucceed ReleaseSequence(const std::string& sequence_name) {
	if (IsEffectSequenceExist(sequence_name) == EFound::NotFound) {
    PUSH_LOG_ERROR_EXT("Not found post-processing sequence. [{} : {}]",
        "Sequence name", sequence_name);
    return ESucceed::Failed;
  }

  // If present post-pocessing is sequence_name, revert it to nullptr.
  if (m_present_sequence == &m_effect_sequences.find(sequence_name)->second)
    m_present_sequence = nullptr;

  // Get count down and erase sequences.
  for (auto& item : m_effect_sequences[sequence_name]) {
    item->Disable();
  }

  m_effect_sequences.erase(sequence_name);
  PUSH_LOG_INFO_EXT("Succeeded in releasing post-processing sequence. [{} : {}]",
      "Sequence name", sequence_name);
  return ESucceed::Succeed;
}

} /// ::opgs16::manager::postprocessing namespace
