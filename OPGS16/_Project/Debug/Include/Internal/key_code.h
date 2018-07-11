#ifndef DEBUG_PROJECT_INTERNAL_KEY_CODE_H
#define DEBUG_PROJECT_INTERNAL_KEY_CODE_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///

namespace debug::keyword {

enum class EKeyboardKeyCode {
  A = 16, B = 17, C = 18, D = 19, E = 20, F = 21, G = 22, H = 23, I = 24,
  J = 25, K = 26, L = 27, M = 28, N = 29, O = 30, P = 31, Q = 32, R = 33,
  S = 34, T = 35, U = 36, V = 37, W = 38, X = 39, Y = 40, Z = 41,

  Num0 = 42, Num1 = 43, Num2 = 44, Num3 = 45, Num4 = 46, Num5 = 47, Num6 = 48,
  Num7 = 49, Num8 = 50, Num9 = 51, Minus = 52, Plus = 53,

  SquareOp = 54, SquareEd = 55, Semicolon = 56, Apostrophe = 57,
  AngleOp = 58, AngleEd = 59, Question = 60,

  ArrowLeft = 61, ArrowUp = 62, ArrowDown = 63, ArrowRight = 64,

  Ctrl = 65, Alt = 66, Tilde = 67, Escape = 68, Pipe = 69, Tab = 70,
  BackSpace = 71, Return = 72, Shift = 73
};

}

#endif /// DEBUG_PROJECT_INTERNAL_KEY_CODE_H