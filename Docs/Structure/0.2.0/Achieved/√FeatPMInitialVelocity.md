# CParticleModuleInitialVelocity 모듈

> 2018-08-07 문서 작성

## 문제

초기 알파치를 설정할 수 있는 `CParticleModuleInitialVelocity` 모듈을 구현한다.

## 제안 & 구현 사항

* 초기에 설정할 수 있는 속도는 $$ (x, y, z) $$  3 축으로 가능하다.
* 속도 역시 각 축에 대해서 Offset 을 줘서, 랜덤하게 속도를 받아내는 것이 가능하다.
* 만약 변화폭 속도의 양이 모두 0 이라면, `base_velocity` 만을 반환한다.

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Add CParticleModule, IntialVelocity"
[feat_particle_system 97f9776] feat: Add CParticleModule, IntialVelocity
 2 files changed, 154 insertions(+)
 create mode 100644 OPGS16/Include/Component/ParticleModule/module_initial_velocity.h
 create mode 100644 OPGS16/Source/Component/ParticleModule/module_initial_velocity.cc
```

