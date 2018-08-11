# ParticleModule hash value 실장

> 2018-08-09 문서 작성

## 문제

해쉬 밸류를 임시땜빵으로 쳐놓으니 매우 보기 안쓰럽다. 고치자

## 제안 & 구현 사항



## 결과

``` powershell
PS D:\Development\OPGS16> git checkout -
Switched to branch 'feat_particle_system'
PS D:\Development\OPGS16> git merge -
Updating b68864a..5517a4a
Fast-forward
 OPGS16/Include/Component/Internal/component.h      |  6 +-
 .../Include/Component/Internal/component_macro.h   | 63 ------------------
 .../Component/Internal/particle_module_base.h      |  1 -
 OPGS16/Include/Component/Internal/renderer_base.h  |  3 -
 .../ParticleModule/module_initial_accelation.h     |  4 +-
 .../ParticleModule/module_initial_alpha.h          |  4 +-
 .../ParticleModule/module_initial_color.h          |  4 +-
 .../Component/ParticleModule/module_initial_size.h |  4 +-
 .../ParticleModule/module_initial_velocity.h       |  4 +-
 .../Component/ParticleModule/module_lifetime.h     |  2 +-
 .../Component/ParticleModule/module_spawn.h        |  1 +
// ...
 OPGS16/Include/Helper/macroes.h                    | 74 +++++++++++++++++++++-
 OPGS16/OPGS16.vcxproj                              |  1 -
 OPGS16/OPGS16.vcxproj.filters                      |  1 -
 24 files changed, 87 insertions(+), 107 deletions(-)
 delete mode 100644 OPGS16/Include/Component/Internal/component_macro.h
```

구현 완료 및 `component_macro.h` 을 `macroes.h` 에 통합시켰다.