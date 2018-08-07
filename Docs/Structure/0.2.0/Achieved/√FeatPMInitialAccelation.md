# CParticleModuleInitialAccelation 모듈

> 2018-08-07 문서 작성

## 문제

파티클 모듈인 `CParticleModuleInitialAccelation` 모듈을 작성한다.

## 제안 & 구현 사항

* 해당 모듈은 생명주기 동안 고정된 가속도를 각 유효한 파티클에 부여한다.
* 이 역시 다른 `Initial` 모듈과 동일하게 각 축마다 `Offset` 을 지정해서 최초 고정 가속도를 `base` 에서 랜덤화시킬 수 있다.

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Add ParticleModule, InitialAccelation"
[feat_particle_system df22b65] feat: Add ParticleModule, InitialAccelation
 2 files changed, 144 insertions(+)
 create mode 100644 OPGS16/Include/Component/ParticleModule/module_initial_accelation.h
 create mode 100644 OPGS16/Source/Component/ParticleModule/module_initial_accelation.cc
```

