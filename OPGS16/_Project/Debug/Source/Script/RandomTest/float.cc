
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

/// Header file
#include "../../../Include/Script/RandomTest/float.h"

#include <limits>
/// Random library
#include <Helper/Math/random.h>
#include <Phitos/Dbg/assert.h>

#include "../../../Include/Object/Common/simplelog.h"

namespace debug::script {

void FloatTest::Initiate() {
  m_log = GetBindObject().Instantiate<object::SimpleLog>("Log", 8);
}

void FloatTest::Update(float delta_time) {
  int32_t set_count = 0;

  for (int32_t i = 0; i < m_test_count; ++i) {
    const auto test = opgs16::random::RandomFloat();
    if (test <= std::numeric_limits<float>::max() &&
        test > std::numeric_limits<float>::lowest()) {
      // Success!
      set_count += 1;

      if (set_count == m_set)
        m_log->PushLog("Float test Set.. " + std::to_string(set_count));
    }
    else {
      // Failure!
      PHITOS_UNEXPECTED_BRANCH();
    }
  }
}

void FloatTest::Destroy() {
  GetBindObject().DestroyChild("Log");
}

} /// ::debug::script namespace