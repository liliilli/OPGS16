#ifndef DEBUG_PROJECT_SCRIPT_RANDOMTEST_VECTOR_DIST_RANGE_H
#define DEBUG_PROJECT_SCRIPT_RANDOMTEST_VECTOR_DIST_RANGE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

#include <Component/script_frame.h>
#include <Helper/Type/vector2.h>
#include <Helper/Type/vectori2.h>

namespace opgs16::component {
class CProcedural2DRenderer;
}

namespace debug::script {

class VectorRangeDistribution final : public opgs16::component::CScriptFrame {
public:
  OP16_SCRIPT_GENERATE(VectorRangeDistribution);

  ///
  /// @brief
  ///
  void InsertVector(const opgs16::DVector2& vector);

private:
  void Start() override final;
  void Update(float delta_time) override final {};

  opgs16::DVectorInt2 GetIndexVector(const opgs16::DVector2& vector);

  opgs16::component::CProcedural2DRenderer* renderer = nullptr;
  const int32_t m_size = 64;
};

} /// ::debug::script namespace

#endif /// DEBUG_PROJECT_SCRIPT_RANDOMTEST_VECTOR_DIST_RANGE_H