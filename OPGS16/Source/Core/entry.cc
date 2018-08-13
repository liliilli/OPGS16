#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Core/entry.cc
///
/// @brief
/// The file has main function that operates actual application.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-03-01 Refactoring.
/// 2018-04-07 Move file to System/Core/Private directory.
/// 2018-05-19 Change the way of calling application initiation.
///

/*! This statements are for checking memory leaks. */
#ifdef false
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
_crtBreakAlloc = 180;
_CrtDumpMemoryLeaks();
#endif

/// ::opgs16::MApplication
#include <Core/application.h>
#include "Helper/Type/quaternion.h"
#include "Headers/import_logger.h"
#include <Helper/Type/vector3.h>
#include "Helper/Type/axis.h"

int main(int argc, char** argv) {
#if defined(_WIN32)
#if defined(_DEBUG)
  ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
#endif
  opgs16::entry::Initiate();

  PUSH_LOG_CRITICAL("FFFF");
  opgs16::DQuaternion test{{270, 32, -64}};
  auto angle = test.GetEulerRotationDegreeAngleVector();
  PUSH_LOG_DEBUG_EXT("Quaternion Degree {}, {}, {}", angle.x, angle.y, angle.z);
  auto mat = test.GetRotationMatrix();
  for (int i = 0; i < 4; ++i) {
    PUSH_LOG_DEBUG_EXT("{}, {}, {}, {}", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }

  test.AddRotationAngle({-16, 45, 0});
  angle = test.GetEulerRotationDegreeAngleVector();
  PUSH_LOG_DEBUG_EXT("Quaternion Degree {}, {}, {}", angle.x, angle.y, angle.z);
  mat = test.GetRotationMatrix();
  for (int i = 0; i < 4; ++i) {
    PUSH_LOG_DEBUG_EXT("{}, {}, {}, {}", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }

  test.AddRotationAngle({0, 45, 0});
  angle = test.GetEulerRotationDegreeAngleVector();
  PUSH_LOG_DEBUG_EXT("Quaternion Degree {}, {}, {}", angle.x, angle.y, angle.z);
  mat = test.GetRotationMatrix();
  for (int i = 0; i < 4; ++i) {
    PUSH_LOG_DEBUG_EXT("{}, {}, {}, {}", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }

  test = opgs16::DQuaternion{{254, 32.f + 45.f + 45.f, -64}};
  angle = test.GetEulerRotationDegreeAngleVector();
  PUSH_LOG_DEBUG_EXT("Quaternion Degree {}, {}, {}", angle.x, angle.y, angle.z);
  mat = test.GetRotationMatrix();
  for (int i = 0; i < 4; ++i) {
    PUSH_LOG_DEBUG_EXT("{}, {}, {}, {}", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }

  test = opgs16::DQuaternion{};
  test.AddRotationAngle(opgs16::EAxis3D::X, 127.f);
  test.AddRotationAngle(opgs16::EAxis3D::X, 127.f);
  test.AddRotationAngle(opgs16::EAxis3D::Y, -12.f);
  test.AddRotationAngle(opgs16::EAxis3D::Y, 124.f);
  test.AddRotationAngle(opgs16::EAxis3D::Z, -64.f);
  angle = test.GetEulerRotationDegreeAngleVector();
  PUSH_LOG_DEBUG_EXT("Quaternion Degree {}, {}, {}", angle.x, angle.y, angle.z);
  mat = test.GetRotationMatrix();
  for (int i = 0; i < 4; ++i) {
    PUSH_LOG_DEBUG_EXT("{}, {}, {}, {}", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }

  PUSH_LOG_CRITICAL("FFFF");

  opgs16::entry::Run();
  return 0;
}
