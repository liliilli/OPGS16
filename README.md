![logo](Docs/Media/opgs16_logo.png)

# OPGS16 (Old Portable Game Simulator 16)

Old Portable Game Simulator 16 (a.k.a OPen Game Simulator 16) is retro-like game engine and puts an intention to make 8-bit and 16-bit games easily. This project is inspired by [PICO8](https://www.lexaloffle.com/pico-8.php). and It is written in C++ and on development now.

You can see what third-party libraries are used, and be able to check what licenses are manifested referring to [COPYRIGHT](./COPYRIGHT) file.

**NOTE**: *I'm working on this project in my spare time, so this project's development activity might not be active. but This project is not abandoned yet.*

# Screenshots

![raining](Docs/Media/opgs16_exam1.gif)

# [License](./LICENSE)

BSD 2-Clause License

Copyright (c) 2018, Jongmin Yun(Neu.)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# To do

1. InputManager
   * <del>stick key 구현</del>
   * 마우스 이동 구현
2. 각 매니지먼트 시스템의 에러 체킹 메소드 구현 및 로거에 바인딩하기
3. 각 후처리 프레임의 로컬 텍스쳐를 TextureManager 에서 가져올 수 있도록 바인딩