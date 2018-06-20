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

- [ ] Font manager (5th)
  - [ ] Resource manager 에서 Default 된 Font 가 없을 경우, 맨 첫번째로 받아온 것을 Default 로 하게 할 것.
  - [x] 리팩터링
  - [x] 확장 ASCII 이외 유니코드 문자 출력 구현
  - [ ] utf-8, utf-16 문자열을 저장하는 커스텀 스트링 컨테이너 구현
  - [ ] utf-8 저장 시, 런타임에 utf-16 문자열로 변환해서 저장하게끔 구현
  - [ ] 커스텀 스트링 컨테이너의 인덱스 참조, 붙이기, 등 구현.
  - [x] 확장 ASCII 이외 유니코드 문자 출력 테스트
    - [x] 테스트 씬 구현하기
  - [x] 다량의 텍스트 출력에 의한 퍼포먼스 저하 개선 (없는 듯한데?)
  - [x] 버그 : 8 pt 와 16 pt 이외의 폰트크기로 표시 시, 표시되지 않는 문제 수정. 우선 렌더러에서 엣지 뽑아낼 때 float floor 처리가 되서 일어나는 문제.
  - [x] 버그 : 개행 간격이 이상한 문제 수정. 및 개행 시마다 폰트가 타버리는 문제 수정. 폰트가 타버리는 문제는 개행 위치를 조정할 때 포인트가 소수점으로 내려가서 일어나는 것. 개행 간격이 이상한 것은 개행 문자를 null 로 잡아버려서 그런 것.
  - [ ] 버그 : 메모리 누수
- [ ] Input manager (1st)
  - [x] Input setting 파일 manifest 을 통해서 불러오게 하기.
  - [x] 마우스 버튼 입력 구현
    - [x] 가상 마우스 출력
    - [ ] 고정 텍스쳐를 입힌 가상 마우스 출력
  - [ ] 마우스 버튼 입력 테스트
    - [x] 일반 로그 출력 테스트.
    - [ ] 실제 오브젝트와 상호작용 테스트
  - [ ] 인풋 키 디버그 씬 구현 및 테스트
    - [ ] 각 콜백에서 입력 값 캐쉬 저장 구현
  - [ ] 키, 마우스 인풋 로그 출력
    - [x] 꾸밈없는 키보드 입력 폴링 출력.
    - [x] 마우스 위치 이동.
    - [ ] 마우스 객체 생성자, 소멸자 붙이기.
    - [x] 마우스 위치 이동 화면 스케일 보정 및 축 좌표 전환 보정.
    - [ ] 마우스 위치 이동 총 테스트 및 검증.
- [ ] Object manager (6th)
  - [x] 코드 스타일에 맞춰서 리팩터링
  - [ ] 씬 내부 오브젝트 Update 처리를 Component 별로 개선
    - [ ] 검증 후에 처리를 해야할 것 같음. (성능 비교 후 처리)
  - [x] 에러 핸들링 검증
  - [ ] /// @todo Improve performance object destruction.
  - [ ] 버그 : 메모리 누수
- [ ] Mesh (3rd)
  - [ ] 3D Mesh obj, fbx 불러오기 구현
  - [ ] VAO, VBO 등의 지오메트리 버퍼 저장소 구현
  - [ ] 메모리 누수 잡기
- [ ] Object (2nd)
  - [x] 생명주기 디버그 및 테스트 씬 만들기
    - [ ] Bug? Object 가 Disabled 되었을 때, 해당 Object 의 스크립트의 타이머는 어떻게 되는가?
    - [x] Camera Orthogonal 시, Z 가 안보이는 문제?
  - [ ] 사양임. Orthogonal 은 기본 rear 와 far 이 0 으로 되어 있음.
    - [x] EmptyObject (말 그대로 텅 빈 오브젝트) 를 만들어서 빌트인으로 제공해야 함.
    - [ ] Bug : Texture2DAtlas Index 반영 안되는 문제
  - [ ] 생명주기 루틴 구현
    - [x] scene Instantiate 부분 수정
    - [x] object Instantiate 부분 수정
  - [x] 생명주기 Initiate() 루틴 구현
    - [x] Scene 구현
    - [x] Object 구현
    - [ ] 코드 리팩터링 및 공통 부분 별도의 파일로 코드 옮기기.
  - [x] 생명주기 Start() 루틴 구현
  - [ ] 생명주기 Disabled() 루틴 구현
    - [ ] Enabled, Disabled 는 Object 자체의 Enable, Disable 전환에 따라 실행됨.
  - [ ] 생명주기 Enabled() 루틴 구현
  - [x] 생명주기 Destroy() 루틴 구현
    - [x] 파괴 리스트에 담긴 후에, 진짜로 힙 릴리즈가 되기 전에 Destroy() 함수 실행.
- [ ] Physical (4th)
  - [ ] 리팩터링.
  - [ ] Kinetic, Dynamic 및 스위치에 따른 루틴 구현
  - [x] bullet library 적용 (x64 Debug)
  - [ ] 새로운 Rigidbody (bullet library 을 적용하는) 을 만들어서 적용.
    - [ ] 충돌박스 표시하기
- [ ] etc (7th)
  - [ ] 파티클 시스템 구현
  - [ ] 빛 시스템 구현 및 검증
  - [ ] Release mode 동작 테스트+'
  - [x] VAO, VBO 와 같은 메쉬 버퍼 컨테이너 내부 관리자 만들기. 및 RAII 로 메모리 누수 처리하면서...
    - [x] 3D 는 아직 하지 말기. 2D 쿼드만 처리하거나, 아니면 2D 형태만 할 수 있도록. 하지만 2D 메쉬도 결국 3D 이기 때문에... 등등.
  - [ ] TimerManager 1프레임 일시정지, 재개 테스트
  - [x] 리소스 파일 구조 바꾸기
- [ ] Object, Component 의 Enable, Disable 처리 기능. (8th 및 onEnable disable 이랑 관계 有)
  - [ ] Object 부분
    - [ ] 기초
    - [ ] 전파
  - [ ] Component 부분
