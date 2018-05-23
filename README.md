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

# Todo list

Updated 2018.05.23

- [ ] Font manager
  - [ ] 리팩터링
  - [ ] 확장 ASCII 이외 유니코드 문자 출력 구현
  - [ ] 확장 ASCII 이외 유니코드 문자 출력 테스트
  - [ ] 다량의 텍스트 출력에 의한 퍼포먼스 저하 개선
- [ ] Input manager
  - [ ] 마우스 버튼 입력 구현
    - [ ] 가상 마우스 출력
  - [ ] 마우스 버튼 입력 테스트
  - [ ] 인풋 키 디버그 씬 구현 및 테스트
  - [ ] 키, 마우스 인풋 로그 출력
    - [ ] 꾸밈없는 키보드 입력 폴링 출력.
    - [x] 마우스 위치 이동.
    - [ ] 마우스 객체 생성자, 소멸자 붙이기.
    - [x] 마우스 위치 이동 화면 스케일 보정 및 축 좌표 전환 보정.
    - [ ] 마우스 위치 이동 총 테스트 및 검증.
- [ ] Object manager
  - [ ] 코드 스타일에 맞춰서 리팩터링
  - [ ] 씬 내부 오브젝트 Update 처리를 Component 별로 개선
  - [ ] 에러 핸들링 검증
- [ ] Mesh
  - [ ] 3D Mesh obj, fbx 불러오기 구현
  - [ ] VAO, VBO 등의 지오메트리 버퍼 저장소 구현
- [ ] Object
  - [ ] 생명주기 루틴 구현
  - [ ] 생명주기 Initiate() 루틴 구현
  - [ ] 생명주기 Start() 루틴 구현
  - [ ] 생명주기 Disabled() 루틴 구현
  - [ ] 생명주기 Enabled() 루틴 구현
  - [ ] 생명주기 Destroy() 루틴 구현
- [ ] Physical
  - [ ] AABB 완전 검증
  - [ ] OOB 구현 및 검증
  - [ ] Kinetic, Dynamic 및 스위치에 따른 루틴 구현
- [ ] etc
  - [ ] 파티클 시스템 구현
  - [ ] 빛 시스템 구현 및 검증
